#include "StereoFilter.h"

StereoFilter::StereoFilter(FilterType type)
{
    for (auto& f : m_filters)
        f.setType(type);
}

void StereoFilter::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock);
    spec.numChannels = 1;
    for (auto& f : m_filters)
        f.prepare(spec);
}

void StereoFilter::reset() noexcept
{
    for (auto& f : m_filters)
        f.reset();
}

void StereoFilter::setCutoff(float freq, Channel channel)
{
    m_filters[channel].setCutoffFrequency(freq);
}

void StereoFilter::setQ(float q)
{
    for (auto& f : m_filters)
        f.setResonance(q);
}

void StereoFilter::processSample(const float& inL, const float& inR, float& outL, float& outR)
{
    outL = m_filters[Channel::Left].processSample(0, inL);
    outR = m_filters[Channel::Right].processSample(0, inR);
}
