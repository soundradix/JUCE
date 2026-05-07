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

#if JUCE_LINUX || JUCE_BSD

namespace juce::detail
{

// Implemented in juce_Messaging_linux.cpp
bool dispatchNextMessageOnSystemQueue (bool returnIfNoPendingMessages);

class MessageThread : public Thread
{
public:
    MessageThread() : Thread (SystemStats::getJUCEVersion() + ": Plugin Message Thread")
    {
        start();
    }

    ~MessageThread() override
    {
        MessageManager::getInstance()->stopDispatchLoop();
        stop();
    }

    void start()
    {
        startThread (Priority::high);

        // Wait for setCurrentThreadAsMessageThread() and getInstance to be executed
        // before leaving this method
        threadInitialised.wait (10000);
    }

    void stop()
    {
        signalThreadShouldExit();
        stopThread (-1);
    }

    bool isRunning() const noexcept  { return isThreadRunning(); }

    void run() override
    {
        MessageManager::getInstance()->setCurrentThreadAsMessageThread();
        XWindowSystem::getInstance();

        threadInitialised.signal();

        while (! threadShouldExit())
        {
            if (! dispatchNextMessageOnSystemQueue (true))
                Thread::sleep (1);
        }
    }

private:
    WaitableEvent threadInitialised;
    JUCE_DECLARE_NON_MOVEABLE (MessageThread)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MessageThread)
};

//==============================================================================
class HostDrivenEventLoop
{
public:
    HostDrivenEventLoop()
    {
        messageThread->stop();
        MessageManager::getInstance()->setCurrentThreadAsMessageThread();
    }

    void processPendingEvents()
    {
        MessageManager::getInstance()->setCurrentThreadAsMessageThread();

        for (;;)
            if (! dispatchNextMessageOnSystemQueue (true))
                return;
    }

    ~HostDrivenEventLoop()
    {
        messageThread->start();
    }

private:
    SharedResourcePointer<MessageThread> messageThread;
};

} // namespace juce::detail

#endif
