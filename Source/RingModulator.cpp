#include "RingModulator.h"

RingModulator::RingModulator()
{
    m_osc.initialise([](float x) { return std::sin(x); }, 128);
}

void RingModulator::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 1;
    m_sampleRate = static_cast<float>(sampleRate);
    m_osc.prepare(spec);
}

void RingModulator::reset() noexcept
{
    m_osc.reset();
}

float RingModulator::processSample(const float& in)
{
    return in * m_osc.processSample(0.0f);
}

void RingModulator::setFrequency(float frequency)
{
    if (!juce::approximatelyEqual(frequency, m_frequency)) {
        m_frequency = frequency;
        m_osc.setFrequency(m_frequency);
    }
}