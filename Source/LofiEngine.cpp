#include "LofiEngine.h"
#include "DSP.h"
LofiEngine::LofiEngine()
{
}

void LofiEngine::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
    m_sampleRate = static_cast<float>(sampleRate);
    m_bitCrusherL.prepareToPlay(m_sampleRate);
    m_bitCrusherR.prepareToPlay(m_sampleRate);
    m_highpass.prepareToPlay(m_sampleRate, samplesPerBlock);
    m_lowpass.prepareToPlay(m_sampleRate, samplesPerBlock);
    m_highpass.setCutoff(100.0f); // remove low-end rumble that happens on low sample rates
    m_highpass.setQ(0.3f);
    m_lowpass.setCutoff(Parameters::maxLofiDampenFreq);
    m_lowpass.setQ(0.2f);
}

void LofiEngine::reset() noexcept
{
    m_bitCrusherL.reset();
    m_bitCrusherR.reset();
    m_highpass.reset();
    m_lowpass.reset();
    m_mixLevel = 1.0f;
    m_downsampleRate = -1.0f;
    m_dampenFreq = -1.0f;
}

void LofiEngine::update(const Parameters& params)
{
    setDownsampleRate(params.lofiSampleRate());
    setMixLevel(params.lofiMixLevel());
    setDampenFreq(params.lofiDampenFreq());
}

void LofiEngine::processSample(const float& inL, const float& inR, float& outL, float& outR)
{
    float processedL = 0.0f;
    float processedR = 0.0f;
    m_highpass.processSample(inL, inR, processedL, processedR);
    processedL = m_bitCrusherL.processSample(processedL);
    processedR = m_bitCrusherR.processSample(processedR);
    m_lowpass.processSample(processedL, processedR, processedL, processedR);
    outL = inL * (1.0f - m_mixLevel) + processedL * m_mixLevel;
    outR = inR * (1.0f - m_mixLevel) + processedR * m_mixLevel;
}

void LofiEngine::setDownsampleRate(float downsampleRate)
{
    if (!juce::approximatelyEqual(downsampleRate, m_downsampleRate)) {
        m_downsampleRate = downsampleRate;
        m_bitCrusherL.setDownsampleRate(m_downsampleRate);
        m_bitCrusherR.setDownsampleRate(m_downsampleRate);
    }
}

void LofiEngine::setDampenFreq(float dampenFreq)
{
    if (!juce::approximatelyEqual(dampenFreq, m_dampenFreq)) {
        m_dampenFreq = dampenFreq;
        m_lowpass.setCutoff(m_dampenFreq);
    }
}

void LofiEngine::setMixLevel(float mixLevel)
{
    m_mixLevel = mixLevel;
}