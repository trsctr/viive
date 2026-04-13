#include "StereoDelay.h"
#include "Parameters.h"
#include "DSP.h"
StereoDelay::StereoDelay()
{
}

void StereoDelay::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = juce::uint32(samplesPerBlock);
	spec.numChannels = 1;
	m_sampleRate = static_cast<float>(sampleRate);
	double numSamples = Parameters::maxDelayTime / 1000.0 * m_sampleRate;
	int maxDelayInSamples = int(std::ceil(numSamples));
	for (int i = 0; i < 2; i++) {
		m_delayLines[i].prepare(spec);
		m_delayLines[i].setMaximumDelayInSamples(maxDelayInSamples);
	}
}

void StereoDelay::reset() noexcept
{
	for (int i = 0; i < 2; i++) {
		m_delayLines[i].reset();
		m_delayTimesMs[i] = -1.0f;
		m_delayTimesSamples[i] = -1.0f;
	}
}

void StereoDelay::setDelayTime(float ms, Channel channel)
{
	if (ms != m_delayTimesMs[channel]) {
		m_delayTimesMs[channel] = ms;
		m_delayTimesSamples[channel] = msToSamples(m_delayTimesMs[channel], m_sampleRate);
		m_delayLines[channel].setDelay(m_delayTimesSamples[channel]);
	}
}

void StereoDelay::processSample(const float& in, float& out, Channel channel)
{
	m_delayLines[channel].pushSample(0, in);
	out = m_delayLines[channel].popSample(0);
}