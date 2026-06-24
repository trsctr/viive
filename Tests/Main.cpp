#include <JuceHeader.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

const char* RED   = "\033[1;31m";
const char* GREEN = "\033[1;32m";
const char* RESET = "\033[0m";

void enableAnsiColors()
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

struct ConsoleLogger final : juce::Logger
{
    void logMessage(const juce::String& message) override
    {
        if (message.startsWith("!!!"))
            std::cout << RED << message << RESET << "\n";
        else
            std::cout << message << "\n";
    }
};

struct ConsoleTestRunner final : juce::UnitTestRunner
{
    void logMessage(const juce::String& message) override
    {
        juce::Logger::writeToLog(message);
    }
};

} // namespace

int main()
{
    enableAnsiColors();

    ConsoleLogger logger;
    juce::Logger::setCurrentLogger(&logger);

    ConsoleTestRunner runner;
    runner.runAllTests();

    int totalPasses = 0, totalFailures = 0;
    juce::StringArray failureSummary;

    for (int i = 0; i < runner.getNumResults(); ++i)
    {
        const auto* result = runner.getResult(i);
        totalPasses   += result->passes;
        totalFailures += result->failures;

        if (result->failures > 0)
        {
            failureSummary.add(result->unitTestName + " / " + result->subcategoryName);
            for (const auto& msg : result->messages)
                if (msg.startsWith("!!!"))
                    failureSummary.add("    " + msg);
        }
    }

    std::cout << "\n================================================================\n";

    if (totalFailures == 0)
    {
        std::cout << GREEN << "ALL TESTS PASSED (" << totalPasses << ")" << RESET << "\n";
    }
    else
    {
        std::cout << RED << "FAILED: " << totalFailures << "  "
                  << GREEN << "PASSED: " << totalPasses << RESET << "\n\n";
        for (const auto& line : failureSummary)
            std::cout << RED << line << RESET << "\n";
    }

    std::cout << "================================================================\n";

    juce::Logger::setCurrentLogger(nullptr);
    juce::DeletedAtShutdown::deleteAll();

    return totalFailures > 0 ? 1 : 0;
}
