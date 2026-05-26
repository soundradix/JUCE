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

#include <JuceHeader.h>

//==============================================================================
class ConsoleLogger final : public Logger
{
    void logMessage (const String& message) override
    {
        std::cout << message << std::endl;

       #if JUCE_WINDOWS
        Logger::outputDebugString (message);
       #endif
    }
};

//==============================================================================
class ConsoleUnitTestRunner final : public UnitTestRunner
{
    void logMessage (const String& message) override
    {
        Logger::writeToLog (message);
    }
};


//==============================================================================
int main (int argc, char **argv)
{
    constexpr auto helpOption = "--help|-h";
    constexpr auto listOption = "--list-categories|-l";
    constexpr auto categoryOption = "--category|-c";
    constexpr auto seedOption = "--seed|-s";
    constexpr auto nameOption = "--name|-n";

    ArgumentList args (argc, argv);

    if (args.containsOption (helpOption))
    {
        std::cout << argv[0]
                  << " [" << helpOption << "]"
                  << " [" << listOption << "]"
                  << " [" << categoryOption << "=category]"
                  << " [" << seedOption << "=seed]"
                  << " [" << nameOption << "=name]"
                  << std::endl;
        return 0;
    }

    if (args.containsOption (listOption))
    {
        for (auto& category : UnitTest::getAllCategories())
            std::cout << category << std::endl;

        return  0;
    }

    ConsoleLogger logger;
    Logger::setCurrentLogger (&logger);

    const ScopeGuard onExit { [&]
    {
        Logger::setCurrentLogger (nullptr);
        DeletedAtShutdown::deleteAll();
    }};

    ConsoleUnitTestRunner runner;

    const auto seed = std::invoke ([&]
    {
        if (args.containsOption (seedOption))
        {
            auto seedValueString = args.getValueForOption (seedOption);

            if (seedValueString.startsWith ("0x"))
                return seedValueString.getHexValue64();

            return seedValueString.getLargeIntValue();
        }

        return Random::getSystemRandom().nextInt64();
    });

    if (args.containsOption (categoryOption) || args.containsOption (nameOption))
    {
        while (args.containsOption (categoryOption))
            runner.runTestsInCategory (args.removeValueForOption (categoryOption), seed);

        while (args.containsOption (nameOption))
            runner.runTestsWithName (args.removeValueForOption (nameOption), seed);
    }
    else
    {
        runner.runAllTests (seed);
    }

    std::vector<String> failures;

    for (int i = 0; i < runner.getNumResults(); ++i)
    {
        auto* result = runner.getResult (i);

        if (result->failures > 0)
        {
            const auto testName = result->unitTestName + " / " + result->subcategoryName;
            const auto testSummary = String (result->failures) + " test failure" + (result->failures > 1 ? "s" : "");
            const auto newLineAndTab = newLine + "\t";

            failures.push_back (testName + ": " + testSummary + newLineAndTab
                                + result->messages.joinIntoString (newLineAndTab));
        }
    }

    logger.writeToLog (newLine + String::repeatedString ("-", 65));

    if (! failures.empty())
    {
        logger.writeToLog ("Test failure summary:");

        for (const auto& failure : failures)
            logger.writeToLog (newLine + failure);

        return 1;
    }

    logger.writeToLog ("All tests completed successfully");
    return 0;
}
