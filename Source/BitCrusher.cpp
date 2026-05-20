#include "BitCrusher.h"

BitCrusher::BitCrusher(float bitDepth, float downsampleRate)
    : m_bitDepth(bitDepth), m_downsampleRate(downsampleRate)
{
}

void BitCrusher::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
    m_sampleRate = static_cast<float>(sampleRate);
    setRatio();
}

void BitCrusher::reset() noexcept
{
    m_currentSample = 0.0f;
    m_accumulator = 0.0f;
}

float BitCrusher::processSample(const float& in)
{
    return downsample(in);
}

float BitCrusher::downsample(const float& sample)
{
    m_accumulator += m_ratio;
    if (m_accumulator >= 1.0f) {
        // DBG("Accumulator before: " << m_accumulator);
        m_accumulator -= 1.0f;
        m_currentSample = sample;
        // DBG("Current sample: " << m_currentSample);
        // DBG("Accumulator after: " << m_accumulator);
        
    }
    return m_currentSample;
}

void BitCrusher::update(const Parameters& params) noexcept
{
    //setBitDepth(params.getBitCrusherBitDepth());
    setDownsampleRate(params.bitCrusherSampleRate());
}

void BitCrusher::setDownsampleRate(float downsampleRate)
{
    if (!juce::approximatelyEqual(downsampleRate, m_downsampleRate)) {
        m_downsampleRate = downsampleRate;
        setRatio();
    }
}

void BitCrusher::setRatio()
{
    m_ratio = m_downsampleRate / m_sampleRate;
}