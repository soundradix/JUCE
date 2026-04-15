/*
  ==============================================================================

   This file is part of the JUCE 9 preview.
   Copyright (c) Raw Material Software Limited

   You may use this code under the terms of the AGPLv3
   (see www.gnu.org/licenses).

   For the JUCE 9 preview this file cannot be licensed commercially.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

struct XFreeDeleter
{
    void operator() (void* ptr) const
    {
        if (ptr != nullptr)
            X11Symbols::getInstance()->xFree (ptr);
    }
};

template <typename Data>
std::unique_ptr<Data, XFreeDeleter> makeXFreePtr (Data* raw) { return std::unique_ptr<Data, XFreeDeleter> (raw); }

//==============================================================================
// Defined in juce_Windowing_linux.cpp
void juce_LinuxAddRepaintListener (ComponentPeer*, Component* dummy);
void juce_LinuxRemoveRepaintListener (ComponentPeer*, Component* dummy);

class PeerListener : private ComponentMovementWatcher
{
public:
    PeerListener (Component& comp, Window embeddedWindow)
        : ComponentMovementWatcher (&comp),
          window (embeddedWindow),
          association (comp.getPeer(), window) {}

private:
    using ComponentMovementWatcher::componentMovedOrResized,
          ComponentMovementWatcher::componentVisibilityChanged;

    void componentMovedOrResized (bool, bool) override {}
    void componentVisibilityChanged() override {}

    void componentPeerChanged() override
    {
        // This should not be rewritten as a ternary expression or similar.
        // The old association must be destroyed before the new one is created.
        association = {};

        if (auto* comp = getComponent())
            association = ScopedWindowAssociation (comp->getPeer(), window);
    }

    Window window{};
    ScopedWindowAssociation association;
};

//==============================================================================
class OpenGLContext::NativeContext
{
private:
    struct DummyComponent  : public Component
    {
        explicit DummyComponent (NativeContext& nativeParentContext)
            : native (nativeParentContext)
        {
        }

        void handleCommandMessage (int commandId) override
        {
            if (commandId == 0)
                native.triggerRepaint();
        }

        NativeContext& native;
    };

    template <typename Traits>
    class ScopedEGLObject
    {
    public:
        using Type = typename Traits::Type;

        ScopedEGLObject() = default;

        ScopedEGLObject (Type obj, EGLDisplay d)
            : object (obj), display (d) {}

        ScopedEGLObject (ScopedEGLObject&& other) noexcept
            : object  (std::exchange (other.object, Type{})),
              display (std::exchange (other.display, EGL_NO_DISPLAY)) {}

        ScopedEGLObject& operator= (ScopedEGLObject&& other) noexcept
        {
            ScopedEGLObject { std::move (other) }.swap (*this);
            return *this;
        }

        ~ScopedEGLObject() noexcept
        {
            if (object != Type{})
                Traits::destroy (display, object);
        }

        Type get() const { return object; }

        void reset() noexcept
        {
            *this = ScopedEGLObject();
        }

        void swap (ScopedEGLObject& other) noexcept
        {
            std::swap (other.object,  object);
            std::swap (other.display, display);
        }

        bool operator== (const ScopedEGLObject& other) const
        {
            const auto tie = [] (const auto& x) { return std::tie (x.object, x.display); };
            return tie (*this) == tie (other);
        }

        bool operator!= (const ScopedEGLObject& other) const
        {
            return ! operator== (other);
        }

    private:
        Type object{};
        EGLDisplay display = EGL_NO_DISPLAY;
    };

    struct TraitsEGLContext
    {
        using Type = EGLContext;

        static void destroy (EGLDisplay display, Type t)
        {
            eglDestroyContext (display, t);
        }
    };

    struct TraitsEGLSurface
    {
        using Type = EGLSurface;

        static void destroy (EGLDisplay display, Type t)
        {
            eglDestroySurface (display, t);
        }
    };

    using PtrEGLContext = ScopedEGLObject<TraitsEGLContext>;
    using PtrEGLSurface = ScopedEGLObject<TraitsEGLSurface>;

public:
    NativeContext (Component& comp,
                   const OpenGLPixelFormat& cPixelFormat,
                   void* shareContext,
                   bool useMultisamplingIn,
                   OpenGLVersion)
        : component (comp), contextToShareWith (shareContext), dummy (*this)
    {
        display = XWindowSystem::getInstance()->getDisplay();

        XWindowSystemUtilities::ScopedXLock xLock;

        X11Symbols::getInstance()->xSync (display, False);

        eglDisplay = eglGetDisplay (display);

        if (eglDisplay == EGL_NO_DISPLAY)
            return;

        {
            EGLint major = 0, minor = 0;

            if (! eglInitialize (eglDisplay, &major, &minor))
                return;
        }

        const EGLint optionalAttribs[]
        {
            EGL_SAMPLE_BUFFERS, useMultisamplingIn ? 1 : 0,
            EGL_SAMPLES,        cPixelFormat.multisamplingLevel
        };

        if (! tryChooseConfig (cPixelFormat, optionalAttribs) && ! tryChooseConfig (cPixelFormat, {}))
            return;

        EGLint nativeVisualId = 0;
        eglGetConfigAttrib (eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &nativeVisualId);

        auto* peer = component.getPeer();
        jassert (peer != nullptr);

        auto windowH = (Window) peer->getNativeHandle();

        auto [visual, depth] = std::invoke ([this, nativeVisualId]() -> std::tuple<Visual*, int>
        {
            XVisualInfo visualInfo{};
            visualInfo.visualid = (VisualID) nativeVisualId;
            int numVisuals = 0;
            auto xVisualInfo = makeXFreePtr (X11Symbols::getInstance()->xGetVisualInfo (display,
                                                                                        VisualIDMask,
                                                                                        &visualInfo,
                                                                                        &numVisuals));

            if (xVisualInfo != nullptr && numVisuals > 0)
                return { xVisualInfo->visual,
                         xVisualInfo->depth };

            return { DefaultVisual (display, DefaultScreen (display)),
                     DefaultDepth  (display, DefaultScreen (display)) };
        });

        auto colourMap = X11Symbols::getInstance()->xCreateColormap (display, windowH, visual, AllocNone);

        XSetWindowAttributes swa;
        swa.colormap = colourMap;
        swa.border_pixel = 0;
        swa.event_mask = embeddedWindowEventMask;

        const auto physicalBounds = getPhysicalBounds();

        embeddedWindow = X11Symbols::getInstance()->xCreateWindow (display,
                                                                   windowH,
                                                                   physicalBounds.getX(),
                                                                   physicalBounds.getY(),
                                                                   (unsigned int) jmax (1, physicalBounds.getWidth()),
                                                                   (unsigned int) jmax (1, physicalBounds.getHeight()),
                                                                   0,
                                                                   depth,
                                                                   InputOutput,
                                                                   visual,
                                                                   CWBorderPixel | CWColormap | CWEventMask,
                                                                   &swa);

        peerListener.emplace (component, embeddedWindow);

        X11Symbols::getInstance()->xMapWindow (display, embeddedWindow);
        X11Symbols::getInstance()->xFreeColormap (display, colourMap);

        X11Symbols::getInstance()->xSync (display, False);

        juce_LinuxAddRepaintListener (peer, &dummy);
    }

    ~NativeContext()
    {
        eglSurface.reset();
        renderContext.reset();

        if (eglDisplay != EGL_NO_DISPLAY)
            eglTerminate (eglDisplay);

        if (auto* peer = component.getPeer())
        {
            juce_LinuxRemoveRepaintListener (peer, &dummy);

            if (embeddedWindow != 0)
            {
                XWindowSystemUtilities::ScopedXLock xLock;

                X11Symbols::getInstance()->xUnmapWindow (display, embeddedWindow);
                X11Symbols::getInstance()->xDestroyWindow (display, embeddedWindow);
                X11Symbols::getInstance()->xSync (display, False);

                XEvent event;
                while (X11Symbols::getInstance()->xCheckWindowEvent (display,
                                                                     embeddedWindow,
                                                                     embeddedWindowEventMask,
                                                                     &event) == True)
                {
                }
            }
        }
    }

    InitResult initialiseOnRenderThread (OpenGLContext& c)
    {
        eglBindAPI (EGL_OPENGL_API);

        const auto components = [&]() -> Optional<Version>
        {
            switch (c.versionRequired)
            {
                case openGL3_2: return Version { 3, 2 };
                case openGL4_1: return Version { 4, 1 };
                case openGL4_3: return Version { 4, 3 };

                case defaultGLVersion: break;
            }

            return {};
        }();

        if (components.hasValue())
        {
           #if JUCE_DEBUG
            constexpr EGLint contextFlags = EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR;
           #else
            constexpr EGLint contextFlags = 0;
           #endif

            const EGLint attribs[]
            {
                EGL_CONTEXT_MAJOR_VERSION_KHR,        components->major,
                EGL_CONTEXT_MINOR_VERSION_KHR,        components->minor,
                EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,  EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
                EGL_CONTEXT_FLAGS_KHR,                contextFlags,
                EGL_NONE
            };

            renderContext = PtrEGLContext { eglCreateContext (eglDisplay, eglConfig, (EGLContext) contextToShareWith, attribs),
                                            eglDisplay };
        }

        if (renderContext == PtrEGLContext{})
        {
            const EGLint attribs[] { EGL_NONE };
            renderContext = PtrEGLContext { eglCreateContext (eglDisplay, eglConfig, (EGLContext) contextToShareWith, attribs),
                                            eglDisplay };
        }

        if (renderContext == PtrEGLContext{})
            return InitResult::fatal;

        eglSurface = PtrEGLSurface { eglCreateWindowSurface (eglDisplay, eglConfig, (EGLNativeWindowType) embeddedWindow, nullptr),
                                     eglDisplay };

        if (eglSurface == PtrEGLSurface{})
            return InitResult::fatal;

        c.makeActive();
        context = &c;
        return InitResult::success;
    }

    void shutdownOnRenderThread()
    {
        context = nullptr;
        deactivateCurrentContext();
        renderContext.reset();
        eglSurface.reset();
    }

    bool makeActive() const noexcept
    {
        return renderContext != PtrEGLContext{}
                 && eglSurface != PtrEGLSurface{}
                 && eglMakeCurrent (eglDisplay, eglSurface.get(), eglSurface.get(), renderContext.get());
    }

    bool isActive() const noexcept
    {
        return eglGetCurrentContext() == renderContext.get() && renderContext != PtrEGLContext{};
    }

    static void deactivateCurrentContext()
    {
        const auto currentDisplay = eglGetCurrentDisplay();

        if (currentDisplay != EGL_NO_DISPLAY)
            eglMakeCurrent (currentDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }

    void swapBuffers()
    {
        eglSwapBuffers (eglDisplay, eglSurface.get());
    }

    Rectangle<int> getPhysicalBounds() const
    {
        if (auto* peer = component.getPeer())
        {
            const auto peerBounds = peer->getAreaCoveredBy (component);
            const auto physicalBounds = peerBounds.toDouble() * peer->getPlatformScaleFactor();
            return physicalBounds.toNearestInt();
        }

        return component.getBounds();
    }

    void updateWindowPosition()
    {
        const auto physicalBounds = getPhysicalBounds();

        XWindowSystemUtilities::ScopedXLock xLock;
        X11Symbols::getInstance()->xMoveResizeWindow (display,
                                                      embeddedWindow,
                                                      physicalBounds.getX(),
                                                      physicalBounds.getY(),
                                                      (unsigned int) jmax (1, physicalBounds.getWidth()),
                                                      (unsigned int) jmax (1, physicalBounds.getHeight()));
    }

    bool setSwapInterval (int numFramesPerSwap)
    {
        if (numFramesPerSwap == swapFrames)
            return true;

        swapFrames = numFramesPerSwap;
        eglSwapInterval (eglDisplay, numFramesPerSwap);
        return true;
    }

    int getSwapInterval() const                 { return swapFrames; }
    bool createdOk() const noexcept             { return true; }
    void* getRawContext() const noexcept        { return renderContext.get(); }
    GLuint getFrameBufferID() const noexcept    { return 0; }

    void triggerRepaint()
    {
        if (context != nullptr)
            context->triggerRepaint();
    }

    struct Locker
    {
        explicit Locker (NativeContext& ctx) : lock (ctx.mutex) {}
        const ScopedLock lock;
    };

    void addListener (NativeContextListener&) {}
    void removeListener (NativeContextListener&) {}

private:
    bool tryChooseConfig (const OpenGLPixelFormat& format, Span<const EGLint> optionalAttribs)
    {
        std::vector<EGLint> allAttribs
        {
            EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
            EGL_RED_SIZE,        format.redBits,
            EGL_GREEN_SIZE,      format.greenBits,
            EGL_BLUE_SIZE,       format.blueBits,
            EGL_ALPHA_SIZE,      format.alphaBits,
            EGL_DEPTH_SIZE,      format.depthBufferBits,
            EGL_STENCIL_SIZE,    format.stencilBufferBits,
        };

        allAttribs.insert (allAttribs.end(), optionalAttribs.begin(), optionalAttribs.end());

        allAttribs.push_back (EGL_NONE);

        EGLint numConfigs = 0;
        return eglChooseConfig (eglDisplay, allAttribs.data(), &eglConfig, 1, &numConfigs) && numConfigs > 0;
    }

    static constexpr int embeddedWindowEventMask = ExposureMask | StructureNotifyMask;

    CriticalSection mutex;
    Component& component;

    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    PtrEGLContext renderContext;
    PtrEGLSurface eglSurface;

    Window embeddedWindow = {};

    std::optional<PeerListener> peerListener;

    int swapFrames = 0;
    EGLConfig eglConfig = nullptr;
    void* contextToShareWith;

    OpenGLContext* context = nullptr;
    DummyComponent dummy;

    ::Display* display = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext)
};

//==============================================================================
bool OpenGLHelpers::isContextActive()
{
    return eglGetCurrentContext() != EGL_NO_CONTEXT;
}

} // namespace juce
