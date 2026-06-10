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
    m_attackCoeff = onePoleLowpassCoeff(m_attackMs, m_sampleRate);
    m_releaseCoeff = onePoleLowpassCoeff(m_releaseMs, m_sampleRate);
}

void LofiEngine::reset() noexcept
{
    m_bitCrusherL.reset();
    m_bitCrusherR.reset();
    m_highpass.reset();
    m_lowpass.reset();
    m_mixLevel = 1.0f;
    m_resampleFreq = -1.0f;
    m_dampenFreq = -1.0f;
    m_envL = 0.0f;
    m_envR = 0.0f;
}

void LofiEngine::kill()
{
    m_bitCrusherL.reset();
    m_bitCrusherR.reset();
    m_highpass.reset();
    m_lowpass.reset();
    m_envL = 0.0f;
    m_envR = 0.0f;
}

void LofiEngine::update(const Parameters& params)
{
    setResampleFreq(params.lofiResampleFreq());
    setMixLevel(params.lofiMixLevel());
    setDampenFreq(params.lofiDampenFreq());
    setNoiseEnabled(params.lofiNoiseEnabled());
}

void LofiEngine::processSample(const float& inL, const float& inR, float& outL, float& outR)
{
    float processedL = 0.0f;
    float processedR = 0.0f;
    m_highpass.processSample(inL, inR, processedL, processedR);
    processedL = m_bitCrusherL.processSample(processedL);
    processedR = m_bitCrusherR.processSample(processedR);
    addNoise(processedL, processedR);
    m_lowpass.processSample(processedL, processedR, processedL, processedR);
    outL = inL * (1.0f - m_mixLevel) + processedL * m_mixLevel;
    outR = inR * (1.0f - m_mixLevel) + processedR * m_mixLevel;
}

void LofiEngine::setResampleFreq(float resampleFreq)
{
    if (!juce::approximatelyEqual(resampleFreq, m_resampleFreq)) {
        m_resampleFreq = resampleFreq;
        m_bitCrusherL.setDownsampleRate(m_resampleFreq);
        m_bitCrusherR.setDownsampleRate(m_resampleFreq);
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

void LofiEngine::addNoise(float& sampleL, float& sampleR)
{
    float absL = std::abs(sampleL);
    float absR = std::abs(sampleR);
    float gateL = (absL > 1e-4f && m_noiseEnabled) ? 1.0f : 0.0f;
    float gateR = (absR > 1e-4f && m_noiseEnabled) ? 1.0f : 0.0f;
    m_envL = onePoleLowpass(gateL, m_envL, gateL > m_envL ? m_attackCoeff : m_releaseCoeff);
    m_envR = onePoleLowpass(gateR, m_envR, gateR > m_envR ? m_attackCoeff : m_releaseCoeff);

    sampleL += noise() * m_noiseScale * m_envL;
    sampleR += noise() * m_noiseScale * m_envR;
}

float LofiEngine::noise()
{
    return m_random.nextFloat() * 2.0f - 1.0f;
}