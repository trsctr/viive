#pragma once

#include <JuceHeader.h>
#include "BitCrusher.h"
#include "Parameters.h"
#include "InsertEffect.h"

class LofiEngine : public InsertEffect
{
public:
    LofiEngine();
    ~LofiEngine() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept override;
    void reset() noexcept override;
    void processSample(const float& inL, const float& inR, float& outL, float& outR) override;
    void update(const Parameters& params) override;

private:
    BitCrusher m_bitCrusherL{ 12.0f, 22050.0f };
    BitCrusher m_bitCrusherR{ 12.0f, 22050.0f };

    float m_sampleRate = 0.0f;
    float m_mixLevel = 1.0f;
    float m_bitDepth = 0.0f;
    float m_downsampleRate = 0.0f;

    void setBitDepth(float bitDepth);
    void setDownsampleRate(float downsampleRate);
    void setMixLevel(float mixLevel);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LofiEngine)
};