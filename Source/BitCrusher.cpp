#include "BitCrusher.h"

BitCrusher::BitCrusher(float bitDepth, float downsampleRate)
    : m_bitDepth(bitDepth), m_downsampleRate(downsampleRate)
{
}

void BitCrusher::prepareToPlay(float sampleRate) noexcept
{
    m_sampleRate = sampleRate;
    setBitDepth(m_bitDepth);
    setDownsampleRate(m_downsampleRate);
}

void BitCrusher::reset() noexcept
{
    m_currentSample = 0.0f;
    m_accumulator = 0.0f;
}

float BitCrusher::processSample(const float& in)
{
    float out = bitReduce(in);
    return downsample(out);
}

float BitCrusher::bitReduce(const float& sample)
{
    float rounded = std::round(sample * m_steps);
    return rounded / m_steps;
}

float BitCrusher::downsample(const float& sample)
{
    m_accumulator += m_ratio;
    if (m_accumulator >= 1.0f) {
        m_accumulator -= 1.0f;
        m_currentSample = sample;
    }
    return m_currentSample;
}

void BitCrusher::setDownsampleRate(float downsampleRate)
{
    m_downsampleRate = downsampleRate;
    if (m_sampleRate > 0.0f) {
        m_ratio = m_downsampleRate / m_sampleRate;
    }
    else {
        m_ratio = 0.0f;
    }
}


void BitCrusher::setBitDepth(float bitDepth)
{
    m_bitDepth = bitDepth;
    m_steps = std::floor((std::pow(2.0f, m_bitDepth)));
}

