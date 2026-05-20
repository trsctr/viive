#include "BitCrusher.h"

BitCrusher::BitCrusher(float bitDepth, float downsampleRate)
    : m_bitDepth(bitDepth), m_downsampleRate(downsampleRate)
{
}

void BitCrusher::prepareToPlay(double sampleRate) noexcept
{
    m_sampleRate = static_cast<float>(sampleRate);
    setRatio();
    setSteps();
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

float BitCrusher::downsample(const float& sample)
{
    m_accumulator += m_ratio;
    if (m_accumulator >= 1.0f) {
        m_accumulator -= 1.0f;
        m_currentSample = sample;
    }
    return m_currentSample;
}

void BitCrusher::update(const Parameters& params) noexcept
{
    setDownsampleRate(params.bitCrusherSampleRate());
    setBitDepth(params.bitCrusherBitDepth());
}

void BitCrusher::setDownsampleRate(float downsampleRate)
{
    if (!juce::approximatelyEqual(downsampleRate, m_downsampleRate)) {
        m_downsampleRate = downsampleRate;
        setRatio();
    }
}

float BitCrusher::bitReduce(const float& sample)
{
    float rounded = std::ceil(sample * m_steps);
    return rounded / m_steps;
}

void BitCrusher::setBitDepth(float bitDepth)
{
    if (!juce::approximatelyEqual(bitDepth, m_bitDepth)) {
        m_bitDepth = bitDepth;
        setSteps();
    }
}

void BitCrusher::setSteps()
{
    m_steps = std::floor((std::pow(2.0f, m_bitDepth)));
}

void BitCrusher::setRatio()
{
    m_ratio = m_downsampleRate / m_sampleRate;
}