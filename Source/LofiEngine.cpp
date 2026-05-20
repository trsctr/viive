#include "LofiEngine.h"

LofiEngine::LofiEngine()
{
}

void LofiEngine::prepareToPlay(double sampleRate) noexcept
{
    m_sampleRate = static_cast<float>(sampleRate);
    m_bitCrusherL.prepareToPlay(m_sampleRate);
    m_bitCrusherR.prepareToPlay(m_sampleRate);
}

void LofiEngine::reset() noexcept
{
    m_bitCrusherL.reset();
    m_bitCrusherR.reset();
}

void LofiEngine::update(const Parameters& params)
{
    setBitDepth(params.lofiBitDepth());
    setDownsampleRate(params.lofiSampleRate());
    setMixLevel(params.lofiMixLevel());
}

void LofiEngine::processSample(const float& inL, const float& inR, float& outL, float& outR)
{
    float processedL = m_bitCrusherL.processSample(inL);
    float processedR = m_bitCrusherR.processSample(inR);
    outL = inL * (1.0f - m_mixLevel) + processedL * m_mixLevel;
    outR = inR * (1.0f - m_mixLevel) + processedR * m_mixLevel;
}

void LofiEngine::setBitDepth(float bitDepth)
{
    if (!juce::approximatelyEqual(bitDepth, m_bitDepth)) {
        m_bitDepth = bitDepth;
        m_bitCrusherL.setBitDepth(m_bitDepth);
        m_bitCrusherR.setBitDepth(m_bitDepth);
    }
}

void LofiEngine::setDownsampleRate(float downsampleRate)
{
    if (!juce::approximatelyEqual(downsampleRate, m_downsampleRate)) {
        m_downsampleRate = downsampleRate;
        m_bitCrusherL.setDownsampleRate(m_downsampleRate);
        m_bitCrusherR.setDownsampleRate(m_downsampleRate);
    }
}

void LofiEngine::setMixLevel(float mixLevel)
{
    m_mixLevel = mixLevel;
}