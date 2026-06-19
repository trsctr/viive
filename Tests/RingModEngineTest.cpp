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

        beginTest("transition from drift to zero: no hard discontinuity");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            engine.reset();
            applyDrift(10.0f);
            engine.update(params);
            float l = 0.0f, r = 0.0f;
            for (int i = 0; i < settleN; ++i)
                engine.processSample(0.5f, 0.5f, l, r);

            applyDrift(0.0f);
            engine.update(params);

            float prevL = l;
            float maxDelta = 0.0f;
            for (int i = 0; i < settleN; ++i)
            {
                engine.processSample(0.5f, 0.5f, l, r);
                maxDelta = juce::jmax(maxDelta, std::abs(l - prevL));
                prevL = l;
            }
            expect(maxDelta < 0.5f,
                "transition caused discontinuity: maxDelta = " + juce::String(maxDelta));
        }

        // The core invariant from the design doc: when drift returns to zero after being
        // active, the oscillators have diverged in phase and won't re-sync on their own.
        // A stereo-blend fade must collapse the output to mono — but gradually, not
        // in a single sample.

        beginTest("zero drift after active drift: not immediately mono");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            engine.reset();

            // Diverge the oscillators.
            applyDrift(10.0f);
            engine.update(params);
            float l = 0.0f, r = 0.0f;
            for (int i = 0; i < settleN; ++i)
                engine.processSample(0.5f, 0.5f, l, r);

            // Return to zero drift.
            applyDrift(0.0f);
            engine.update(params);

            // The first sample must still be stereo — the blend should not snap
            // to mono in a single step.
            engine.processSample(0.5f, 0.5f, l, r);
            expect(l != r, "blend must not collapse to mono in a single sample");
        }

        beginTest("zero drift after active drift: eventually converges to mono");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            engine.reset();

            // Diverge the oscillators.
            applyDrift(10.0f);
            engine.update(params);
            float l = 0.0f, r = 0.0f;
            for (int i = 0; i < settleN; ++i)
                engine.processSample(0.5f, 0.5f, l, r);

            // Return to zero drift and run long enough for any reasonable
            // blend time constant (up to ~300 ms) to settle within 1 %.
            applyDrift(0.0f);
            engine.update(params);
            for (int i = 0; i < 100'000; ++i)
                engine.processSample(0.5f, 0.5f, l, r);

            expect(std::abs(l - r) < 0.01f,
                "channels did not converge to mono; diff = " + juce::String(std::abs(l - r)));
        }

        beginTest("nonzero drift after settled zero: not immediately stereo");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            engine.reset();

            // Let the blend settle at zero.
            applyDrift(0.0f);
            engine.update(params);
            float l = 0.0f, r = 0.0f;
            for (int i = 0; i < settleN; ++i)
                engine.processSample(0.5f, 0.5f, l, r);

            // Turn drift on — blend should not snap to stereo in one sample.
            applyDrift(10.0f);
            engine.update(params);
            engine.processSample(0.5f, 0.5f, l, r);
            expect(std::abs(l - r) < 0.01f,
                "blend snapped to stereo immediately; diff = " + juce::String(std::abs(l - r)));
        }

        beginTest("nonzero drift after settled zero: eventually becomes stereo");
        {
            RingModEngine engine;
            engine.prepareToPlay(sampleRate, blockSize);
            engine.reset();

            // Let the blend settle at zero.
            applyDrift(0.0f);
            engine.update(params);
            float l = 0.0f, r = 0.0f;
            for (int i = 0; i < settleN; ++i)
                engine.processSample(0.5f, 0.5f, l, r);

            // Turn drift on and let the blend rise.
            applyDrift(10.0f);
            engine.update(params);
            for (int i = 0; i < settleN; ++i)
                engine.processSample(0.5f, 0.5f, l, r);

            expect(l != r, "channels should differ after blend settles with nonzero drift");
        }
    }
};

static RingModEngineTests ringModEngineTests;
