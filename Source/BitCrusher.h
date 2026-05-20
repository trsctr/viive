#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class BitCrusher
{
public:
    BitCrusher(float bitDepth = 24.0f, float downsampleRate = 44100.0f);
    ~BitCrusher() = default;

    void prepareToPlay(float sampleRate) noexcept;
    void reset() noexcept;
    float processSample(const float& in);
    float bitReduce(const float& sample);
    float downsample(const float& sample);
    void setBitDepth(float bitDepth);
    void setDownsampleRate(float downsampleRate);

private:
    float m_sampleRate = 0.0f;
    float m_bitDepth = 0.0f;
    float m_downsampleRate = 0.0f;
    float m_ratio = 0.0f;
    float m_currentSample = 0.0f;
    float m_accumulator = 0.0f;
    float m_steps = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitCrusher)
};

