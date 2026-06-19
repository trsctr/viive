#include <JuceHeader.h>
#include <iostream>

namespace {

struct ConsoleLogger final : juce::Logger
{
    void logMessage(const juce::String& message) override
    {
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
    ConsoleLogger logger;
    juce::Logger::setCurrentLogger(&logger);

    ConsoleTestRunner runner;
    runner.runAllTests();

    int totalFailures = 0;
    for (int i = 0; i < runner.getNumResults(); ++i)
        totalFailures += runner.getResult(i)->failures;

    juce::Logger::setCurrentLogger(nullptr);
    juce::DeletedAtShutdown::deleteAll();

    return totalFailures > 0 ? 1 : 0;
}
