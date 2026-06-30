#pragma once
#include <JuceHeader.h>

class RingModulator
{
public:
    RingModulator();
    ~RingModulator() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
    void reset() noexcept;
    
    float processSample(const float& in);
    void setFrequency(float frequency);

private:
    juce::dsp::Oscillator<float> m_osc;

    float m_sampleRate = 0.0f;
    
    float m_frequency = 440.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingModulator)
};