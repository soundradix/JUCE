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

//==============================================================================
namespace Android
{
    class Runnable : public juce::AndroidInterfaceImplementer
    {
    public:
        virtual void run() = 0;

    private:
        jobject invoke (jobject proxy, jobject method, jobjectArray args) override
        {
            auto* env = getEnv();
            auto methodName = juce::juceString ((jstring) env->CallObjectMethod (method, JavaMethod.getName));

            if (methodName == "run")
            {
                run();
                return nullptr;
            }

            // invoke base class
            return AndroidInterfaceImplementer::invoke (proxy, method, args);
        }
    };

    struct Handler
    {
        Handler() = default;
        ~Handler() { clearSingletonInstance(); }

        JUCE_DECLARE_SINGLETON_INLINE (Handler, false)

        bool post (jobject runnable)
        {
            return (getEnv()->CallBooleanMethod (nativeHandler.get(), AndroidHandler.post, runnable) != 0);
        }

        GlobalRef nativeHandler { LocalRef<jobject> { getEnv()->NewObject (AndroidHandler, AndroidHandler.constructor) } };
    };
}

//==============================================================================
struct AndroidMessageQueue final : private Android::Runnable
{
    JUCE_DECLARE_SINGLETON_SINGLETHREADED_INLINE (AndroidMessageQueue, true)

    AndroidMessageQueue() = default;

    ~AndroidMessageQueue() override
    {
        JUCE_ASSERT_MESSAGE_THREAD
        clearSingletonInstance();
    }

    bool post (MessageManager::MessageBase::Ptr&& message)
    {
        queue.add (std::move (message));

        // this will call us on the message thread
        return handler.post (self.get());
    }

private:

    void run() override
    {
        for (;;)
        {
            MessageManager::MessageBase::Ptr message (queue.removeAndReturn (0));

            if (message == nullptr)
                break;

            message->messageCallback();
        }
    }

    ReferenceCountedArray<MessageManager::MessageBase, CriticalSection> queue;
    Android::Handler handler;

    // the this pointer to this class in Java land
    GlobalRef self { CreateJavaInterface (this, "java/lang/Runnable") };
};

//==============================================================================
void MessageManager::doPlatformSpecificInitialisation() { AndroidMessageQueue::getInstance(); }
void MessageManager::doPlatformSpecificShutdown()       { AndroidMessageQueue::deleteInstance(); }

bool MessageManager::postMessageToSystemQueue (MessageManager::MessageBase* const message)
{
    return AndroidMessageQueue::getInstance()->post (message);
}

//==============================================================================
void MessageManager::broadcastMessage (const String&)
{
}

void MessageManager::runDispatchLoop()
{
}

void MessageManager::stopDispatchLoop()
{
    struct QuitCallback final : public CallbackMessage
    {
        QuitCallback() {}

        void messageCallback() override
        {
            auto* env = getEnv();
            auto activity = getCurrentActivity();

            if (activity != nullptr)
            {
                jmethodID quitMethod = env->GetMethodID (AndroidActivity, "finishAndRemoveTask", "()V");

                if (quitMethod != nullptr)
                {
                    env->CallVoidMethod (activity.get(), quitMethod);
                    return;
                }

                quitMethod = env->GetMethodID (AndroidActivity, "finish", "()V");
                jassert (quitMethod != nullptr);
                env->CallVoidMethod (activity.get(), quitMethod);
            }
            else
            {
                jassertfalse;
            }
        }
    };

    (new QuitCallback())->post();
    quitMessagePosted = true;
}

//==============================================================================
class JuceAppLifecycle final : public ActivityLifecycleCallbacks
{
public:
    using CreateApp = juce::JUCEApplicationBase* (*)();

    explicit JuceAppLifecycle (CreateApp initSymbolAddr)
        : createApplicationSymbol (initSymbolAddr)
    {
    }

    void onActivityCreated (jobject, jobject) override
    {
        checkCreated();
    }

    void onActivityDestroyed (jobject activity) override
    {
        auto* env = getEnv();

        // if the main activity is being destroyed, only then tear-down JUCE
        if (env->IsSameObject (getMainActivity().get(), activity) != 0)
        {
            JUCEApplicationBase::appWillTerminateByForce();
            JNIClassBase::releaseAllClasses (env);

            LocalRef<jclass> systemClass { env->FindClass ("java/lang/System") };
            jmethodID exitMethod = env->GetStaticMethodID (systemClass.get(), "exit", "(I)V");
            env->CallStaticVoidMethod (systemClass, exitMethod, 0);
        }
    }

    void onActivityStarted (jobject) override
    {
        checkCreated();
    }

    void onActivityPaused (jobject) override
    {
        if (auto* app = JUCEApplicationBase::getInstance())
            app->suspended();
    }

    void onActivityResumed (jobject) override
    {
        checkInitialised();

        if (auto* app = JUCEApplicationBase::getInstance())
            app->resumed();
    }

    static JuceAppLifecycle& getInstance (CreateApp initSymbolAddr)
    {
        static JuceAppLifecycle juceAppLifecycle (initSymbolAddr);
        return juceAppLifecycle;
    }

private:
    void checkCreated()
    {
        if (JUCEApplicationBase::getInstance() == nullptr)
        {
            DBG (SystemStats::getJUCEVersion());

            JUCEApplicationBase::createInstance = createApplicationSymbol;

            initialiser.emplace();

            if (! JUCEApplicationBase::createInstance())
                jassertfalse; // you must supply an application object for an android app!

            jassert (MessageManager::getInstance()->isThisTheMessageThread());
        }
    }

    void checkInitialised()
    {
        checkCreated();

        if (! hasBeenInitialised)
        {
            if (auto* app = JUCEApplicationBase::getInstance())
            {
                hasBeenInitialised = app->initialiseApp();

                if (! hasBeenInitialised)
                    exit (app->shutdownApp());
            }
        }
    }

    std::optional<ScopedJuceInitialiser_GUI> initialiser;
    GlobalRef myself;
    CreateApp createApplicationSymbol{};
    bool hasBeenInitialised = false;
    ActivityLifecycleCallbackForwarder forwarder { GlobalRef { getAppContext() }, this };
};

//==============================================================================
File juce_getExecutableFile();

void juce_juceEventsAndroidStartApp();
void juce_juceEventsAndroidStartApp()
{
    auto dllPath = juce_getExecutableFile().getFullPathName();
    auto addr = reinterpret_cast<juce::JUCEApplicationBase* (*)()> (DynamicLibrary (dllPath)
                                                                     .getFunction ("juce_CreateApplication"));

    if (addr != nullptr)
        JuceAppLifecycle::getInstance (addr);
}

} // namespace juce
