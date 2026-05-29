#include "RingModEngine.h"

RingModEngine::RingModEngine()
{
}

void RingModEngine::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
    m_sampleRate = static_cast<float>(sampleRate);
    m_ringModL.prepareToPlay(sampleRate, samplesPerBlock);
    m_ringModR.prepareToPlay(sampleRate, samplesPerBlock);
    m_lfoL.prepareToPlay(m_sampleRate);
    m_lfoR.prepareToPlay(m_sampleRate);
}

void RingModEngine::reset() noexcept
{
    m_ringModL.reset();
    m_ringModR.reset();
    m_lfoL.reset();
    m_lfoR.reset();
}

void RingModEngine::update(const Parameters& params)
{
}

void RingModEngine::kill()
{
    m_ringModL.reset();
    m_ringModR.reset();
    m_lfoL.reset();
    m_lfoR.reset();
}

void RingModEngine::processSample(const float& inL, const float& inR, float& outL, float& outR)
{
    float modSignalL = m_lfoL.process();
    float modSignalR = m_lfoR.process();

    m_ringModL.setFrequency(m_baseFrequency + modSignalL * m_driftAmount);
    m_ringModR.setFrequency(m_baseFrequency + modSignalR * m_driftAmount);

    float processedL = m_ringModL.processSample(inL);
    float processedR = m_ringModR.processSample(inR);

    outL = inL + (processedL - inL) * m_mixLevel;
    outR = inR + (processedR - inR) * m_mixLevel;
}