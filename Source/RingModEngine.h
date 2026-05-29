#pragma once

#include "InsertEffect.h"
#include "RingModulator.h"
#include "LFO.h"

class RingModEngine : public InsertEffect
{
public:
    RingModEngine();
    ~RingModEngine() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept override;
    void reset() noexcept override;
    void update(const Parameters& params) override;
    void kill() override;

    void processSample(const float& inL, const float& inR, float& outL, float& outR) override;

private:
    RingModulator m_ringModL, m_ringModR;

    LFO m_lfoL { .3f, 0.0f, LFOShape::Sine };
    LFO m_lfoR { .3f, 0.5f, LFOShape::Sine };

    float m_sampleRate = 0.0f;

    float m_mixLevel = 1.0f;
    float m_baseFrequency = 440.0f;
    float m_driftAmount = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingModEngine)
};