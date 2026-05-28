#pragma once

#include <JuceHeader.h>
#include "BitCrusher.h"
#include "Parameters.h"
#include "InsertEffect.h"
#include "StereoFilter.h"


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
    BitCrusher m_bitCrusherL{ 8.0f, 22050.0f };
    BitCrusher m_bitCrusherR{ 8.0f, 22050.0f };
    StereoFilter m_highpass { FilterType::highpass };
    StereoFilter m_lowpass { FilterType::lowpass };
    juce::Random m_random;
    
    float m_sampleRate = 0.0f;
    float m_mixLevel = 1.0f;
    float m_resampleFreq = -1.0f;
    float m_dampenFreq = -1.0f;
    float m_envL = 0.0f;
    float m_envR = 0.0f;
    float m_envCoeff = 0.0f;
    float m_noiseScale = 0.001f;

    void setResampleFreq(float resampleFreq);
    void setMixLevel(float mixLevel);
    void setDampenFreq(float dampenFreq);
    void addNoise(float& sampleL, float& sampleR);
    float noise();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LofiEngine)
};