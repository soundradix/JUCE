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

#pragma once

namespace juce::etw
{

//==============================================================================
/*
    The following XML can be passed to Windows Performance Recorder (WPR) to enable tracing from
    JUCE projects.
    - Save the XML into a file with the name JUCE.wprp
    - Run the following command from an admin command prompt to start capture. If you use a Developer
      Command Prompt, wpr.exe should be found on the PATH already.
        - <path to wpr>\wpr.exe -start JUCE.wprp
    - Start your JUCE project
    - Run the following command from an admin command prompt to stop capture
        - <path to wpr>\wpr.exe -stop TraceCaptureFile.etl description

<?xml version="1.0" encoding="utf-8"?>
<WindowsPerformanceRecorder Version="1.0" Author="Microsoft Corporation" Copyright="Microsoft Corporation" Company="Microsoft Corporation">
  <Profiles>
    <EventCollector Id="EventCollector_JUCETraceLogProvider" Name="JUCETraceLogProvider">
      <BufferSize Value="64" />
      <Buffers Value="4" />
    </EventCollector>

    <EventProvider Id="EventProvider_JUCETraceLogProvider" Name="6A612E78-284D-4DDB-877A-5F521EB33132" />

    <Profile Id="JUCETraceLogProvider.Verbose.File" Name="JUCETraceLogProvider" Description="JUCETraceLogProvider" LoggingMode="File" DetailLevel="Verbose">
      <Collectors>
        <EventCollectorId Value="EventCollector_JUCETraceLogProvider">
          <EventProviders>
            <EventProviderId Value="EventProvider_JUCETraceLogProvider" />
          </EventProviders>
        </EventCollectorId>
      </Collectors>
    </Profile>

    <Profile Id="JUCETraceLogProvider.Light.File" Name="JUCETraceLogProvider" Description="JUCETraceLogProvider" Base="JUCETraceLogProvider.Verbose.File" LoggingMode="File" DetailLevel="Light" />
    <Profile Id="JUCETraceLogProvider.Verbose.Memory" Name="JUCETraceLogProvider" Description="JUCETraceLogProvider" Base="JUCETraceLogProvider.Verbose.File" LoggingMode="Memory" DetailLevel="Verbose" />
    <Profile Id="JUCETraceLogProvider.Light.Memory" Name="JUCETraceLogProvider" Description="JUCETraceLogProvider" Base="JUCETraceLogProvider.Verbose.File" LoggingMode="Memory" DetailLevel="Light" />

  </Profiles>
</WindowsPerformanceRecorder>

*/

//==============================================================================
enum
{
    paintKeyword            = 1 << 0,
    sizeKeyword             = 1 << 1,
    graphicsKeyword         = 1 << 2,
    crucialKeyword          = 1 << 3,
    threadPaintKeyword      = 1 << 4,
    messageKeyword          = 1 << 5,
    direct2dKeyword         = 1 << 6,
    softwareRendererKeyword = 1 << 7,
    resourcesKeyword        = 1 << 8,
    componentKeyword        = 1 << 9,
    spriteKeyword           = 1 << 10
};

enum : uint64_t
{
    direct2dHwndPaintStart,
    direct2dHwndPaintEnd,
    endDraw,
    present1SwapChainStart,
    present1SwapChainEnd,
    swapChainThreadEvent,
    waitForVBlankDone,
    callVBlankListeners,
    resize,
    createResource,
    presentIdleFrame,
    direct2dImagePaintStart,
    direct2dImagePaintEnd,
    startD2DFrame,
    flush,
    startGDIFrame,
    startGDIImage,
    endGDIFrame,
    createLowLevelGraphicsContext,
    createDeviceResources,
    createSwapChain,
    createSwapChainBuffer,
    createPeer,
    mapBitmap,
    unmapBitmap,
    createDirect2DBitmapFromImage,
    createDirect2DBitmap,
    setOrigin,
    addTransform,
    clipToRectangle,
    clipToRectangleList,
    excludeClipRectangle,
    clipToPath,
    clipToImageAlpha,
    saveState,
    restoreState,
    beginTransparencyLayer,
    endTransparencyLayer,
    setFill,
    setOpacity,
    setInterpolationQuality,
    fillRect,
    fillRectReplace,
    fillRectList,
    drawRectTranslated,
    drawRectTransformed,
    drawRect,
    fillPath,
    strokePath,
    drawPath,
    drawImage,
    drawLine,
    setFont,
    drawGlyph,
    drawGlyphRun,
    drawTextLayout,
    drawRoundedRectangle,
    fillRoundedRectangle,
    drawEllipse,
    fillEllipse,
    filledGeometryRealizationCacheHit,
    filledGeometryRealizationCreated,
    strokedGeometryRealizationCacheHit,
    strokedGeometryRealizationCreated,
    releaseGeometryRealization,
    gradientCacheHit,
    gradientCreated,
    releaseGradient,
    nativeDropShadow,
    nativeGlowEffect,
    resetToDefaultState,
    reduceClipRegionRectangle,
    reduceClipRegionRectangleList,
    reduceClipRegionImage,
    reduceClipRegionPath,
    excludeClipRegion,
    fillAll,
    repaint,
    paintComponentAndChildren,
    paintWithinParentContext,
    createSpriteBatch,
    setSprites,
    addSprites,
    drawSprites,
};

#if JUCE_WINDOWS && JUCE_ETW_TRACELOGGING
    JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wc++98-compat-extra-semi", "-Wmissing-prototypes", "-Wgnu-zero-variadic-macro-arguments")
    TRACELOGGING_DECLARE_PROVIDER (JUCETraceLogProvider);
    JUCE_END_IGNORE_WARNINGS_GCC_LIKE

    #define JUCE_WRITE_TRACE_LOG_VA(code, keyword, ...) \
        TraceLoggingWrite (::juce::etw::JUCETraceLogProvider, \
                           #code, \
                           TraceLoggingLevel (TRACE_LEVEL_INFORMATION), \
                           TraceLoggingKeyword ((UINT64) keyword), \
                           __VA_ARGS__, \
                           TraceLoggingValue ((unsigned short) code, "code"))
    #define JUCE_WRITE_TRACE_LOG(code, keyword) \
        TraceLoggingWrite (::juce::etw::JUCETraceLogProvider, \
                           #code, \
                           TraceLoggingLevel (TRACE_LEVEL_INFORMATION), \
                           TraceLoggingKeyword ((UINT64) keyword), \
                           TraceLoggingValue ((unsigned short) code, "code"))
#else
    #define JUCE_WRITE_TRACE_LOG_VA(code, keyword, ...)
    #define JUCE_WRITE_TRACE_LOG(code, keyword)
#endif

template <typename Number>
auto toVector (const Rectangle<Number>& r)
{
    return std::vector { r.getX(), r.getY(), r.getWidth(), r.getHeight() };
}

template <typename Number>
auto toVector (const RectangleList<Number>& list)
{
    std::vector<Number> result;

    for (const auto& r : list)
        result.insert (result.end(), { r.getX(), r.getY(), r.getWidth(), r.getHeight() });

    return result;
}

//==============================================================================
#if JUCE_WINDOWS && JUCE_ETW_TRACELOGGING
    #define JUCE_SCOPED_TRACE_EVENT_FRAME(code, keyword, frameNumber) \
        const ScopeGuard scopedEvent_ ## __LINE__ { [start = ::juce::Time::getHighResolutionTicks(), frame = (frameNumber)] \
        {                                                    \
            const auto ticks = ::juce::Time::getHighResolutionTicks() - start; \
            JUCE_WRITE_TRACE_LOG_VA (code, keyword, TraceLoggingValue (ticks), TraceLoggingValue (frame)); \
        } };

    #define JUCE_SCOPED_TRACE_EVENT_FRAME_RECT_F32(code, keyword, frameNumber, rectIn) \
        const ScopeGuard scopedEvent_ ## __LINE__ { [start = ::juce::Time::getHighResolutionTicks(), frame = (frameNumber), rect = (rectIn)] \
        { \
            const auto ticks = ::juce::Time::getHighResolutionTicks() - start;       \
            const std::vector<float> vec = ::juce::etw::toVector (rect); \
            JUCE_WRITE_TRACE_LOG_VA (code,                                             \
                                     keyword,                                          \
                                     TraceLoggingValue (ticks),                        \
                                     TraceLoggingValue (frame),                        \
                                     TraceLoggingFloat32Array (vec.data(), (UINT16) vec.size(), "rect")); \
        } };

    #define JUCE_SCOPED_TRACE_EVENT_FRAME_RECT_I32(code, keyword, frameNumber, rectIn) \
        const ScopeGuard scopedEvent_ ## __LINE__ { [start = ::juce::Time::getHighResolutionTicks(), frame = (frameNumber), rect = (rectIn)] \
        { \
            const auto ticks = ::juce::Time::getHighResolutionTicks() - start;       \
            const std::vector<INT32> vec = ::juce::etw::toVector (rect); \
            JUCE_WRITE_TRACE_LOG_VA (code,                                             \
                                     keyword,                                          \
                                     TraceLoggingValue (ticks),                        \
                                     TraceLoggingValue (frame),                        \
                                     TraceLoggingInt32Array (vec.data(), (UINT16) vec.size(), "rect")); \
        } };
#else
    #define JUCE_SCOPED_TRACE_EVENT_FRAME(code, keyword, frameNumber)
    #define JUCE_SCOPED_TRACE_EVENT_FRAME_RECT_F32(code, keyword, frameNumber, rectIn)
    #define JUCE_SCOPED_TRACE_EVENT_FRAME_RECT_I32(code, keyword, frameNumber, rectIn)
#endif

//==============================================================================
#define JUCE_TRACE_LOG_D2D_PAINT_CALL(code, frameNumber) \
    JUCE_WRITE_TRACE_LOG_VA (code, etw::paintKeyword | etw::direct2dKeyword, TraceLoggingValue ((UINT64) frameNumber, "frame"))

#define JUCE_TRACE_LOG_JUCE_VBLANK_THREAD_EVENT \
    JUCE_WRITE_TRACE_LOG (etw::waitForVBlankDone, etw::softwareRendererKeyword)

#define JUCE_TRACE_LOG_JUCE_VBLANK_CALL_LISTENERS \
    JUCE_WRITE_TRACE_LOG (etw::callVBlankListeners, etw::softwareRendererKeyword)

#define JUCE_TRACE_LOG_D2D_RESIZE(message) \
    JUCE_WRITE_TRACE_LOG_VA (etw::resize, etw::paintKeyword | etw::direct2dKeyword, TraceLoggingValue (message, "message"))

#define JUCE_TRACE_LOG_D2D_IMAGE_MAP_DATA \
    JUCE_WRITE_TRACE_LOG (etw::mapBitmap, etw::direct2dKeyword)

#define JUCE_TRACE_LOG_D2D_IMAGE_UNMAP_DATA \
    JUCE_WRITE_TRACE_LOG (etw::unmapBitmap, etw::direct2dKeyword)

#define JUCE_TRACE_LOG_PAINT_COMPONENT_AND_CHILDREN(depth) \
    JUCE_WRITE_TRACE_LOG_VA (etw::paintComponentAndChildren, etw::paintKeyword, TraceLoggingValue (depth, "depth"))

#define JUCE_TRACE_LOG_PAINT_CALL(code, frameNumber) \
    JUCE_WRITE_TRACE_LOG_VA (code, etw::softwareRendererKeyword, TraceLoggingValue ((UINT64) frameNumber, "frame"))

#if JUCE_ETW_TRACELOGGING
    #define JUCE_TRACE_EVENT_INT_RECT_LIST(code, keyword, frameNumber, rect) \
        { \
            const std::vector<INT32> vec = ::juce::etw::toVector (rect); \
            JUCE_WRITE_TRACE_LOG_VA (code,                              \
                                     etw::softwareRendererKeyword,      \
                                     TraceLoggingValue ((UINT64) frameNumber, "frame"), \
                                     TraceLoggingInt32Array (vec.data(), (UINT16) vec.size(), "rect")); \
        }

    #define JUCE_TRACE_EVENT_INT_RECT(code, keyword, rect) \
        { \
            const std::vector<INT32> vec = ::juce::etw::toVector (rect); \
            JUCE_WRITE_TRACE_LOG_VA (code,                              \
                                     etw::softwareRendererKeyword,      \
                                     TraceLoggingInt32Array (vec.data(), (UINT16) vec.size(), "rect")); \
        }
#else
    #define JUCE_TRACE_EVENT_INT_RECT_LIST(code, keyword, frameNumber, rect)
    #define JUCE_TRACE_EVENT_INT_RECT(code, keyword, rect)
#endif

} // namespace juce::etw
