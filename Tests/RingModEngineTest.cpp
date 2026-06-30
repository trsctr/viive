#include <JuceHeader.h>
#include "../Source/RingModEngine.h"
#include "../Source/Parameters.h"

namespace {

struct MockAudioProcessor : juce::AudioProcessor
{
    MockAudioProcessor()
        : juce::AudioProcessor(BusesProperties()
            .withInput("In", juce::AudioChannelSet::stereo())
            .withOutput("Out", juce::AudioChannelSet::stereo())) {}

    const juce::String getName() const override { return {}; }
    void prepareToPlay(double, int) override {}
    void releaseResources() override {}
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override {}
    double getTailLengthSeconds() const override { return 0.0; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}
};

} // namespace

struct RingModEngineTests : juce::UnitTest
{
    RingModEngineTests() : juce::UnitTest("RingModEngine") {}

    void runTest() override
    {
        constexpr double sampleRate = 44100.0;
        constexpr int blockSize = 512;
        constexpr int settleN = 10000;

        beginTest("no NaN or inf after prepareToPlay + processSample");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            float l = 0.0f, r = 0.0f;
            engine.processSample(0.5f, 0.5f, l, r);
            expect(!std::isnan(l) && !std::isinf(l), "left channel is NaN or inf");
            expect(!std::isnan(r) && !std::isinf(r), "right channel is NaN or inf");
        }

        beginTest("no NaN or inf after reset + processSample");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            engine.reset();
            float l = 0.0f, r = 0.0f;
            engine.processSample(0.5f, 0.5f, l, r);
            expect(!std::isnan(l) && !std::isinf(l), "left channel is NaN or inf");
            expect(!std::isnan(r) && !std::isinf(r), "right channel is NaN or inf");
        }

        beginTest("silence in produces silence out");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            engine.reset();
            float l = 1.0f, r = 1.0f;
            engine.processSample(0.0f, 0.0f, l, r);
            expectEquals(l, 0.0f);
            expectEquals(r, 0.0f);
        }

        beginTest("zero drift: L and R channels are identical");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            engine.reset();
            float l = 0.0f, r = 0.0f;
            for (int i = 0; i < settleN; ++i)
                engine.processSample(0.5f, 0.5f, l, r);
            expectEquals(l, r);
        }

        // Tests below need to drive drift via Parameters + APVTS.
        MockAudioProcessor proc;
        juce::AudioProcessorValueTreeState apvts{
            proc, nullptr, "PARAMS", Parameters::createParameterLayout()
        };
        Parameters params{ apvts };
        params.prepareToPlay(sampleRate);

        // Snap Parameters to a given drift amount in Hz (bypasses smoothing).
        auto applyDrift = [&](float driftHz)
        {
            auto* p = dynamic_cast<juce::AudioParameterFloat*>(
                apvts.getParameter(ringModDriftAmtParamID.getParamID()));
            p->setValueNotifyingHost(p->convertTo0to1(driftHz));
            params.reset();
            params.smoothen();
        };

        beginTest("nonzero drift: L and R channels differ after settling");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            engine.reset();
            applyDrift(10.0f);
            engine.update(params);
            float l = 0.0f, r = 0.0f;
            for (int i = 0; i < settleN; ++i)
                engine.processSample(0.5f, 0.5f, l, r);
            expect(l != r, "channels should differ with nonzero drift");
        }

    }
};

static RingModEngineTests ringModEngineTests;
