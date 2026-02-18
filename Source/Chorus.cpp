#include "Chorus.h"
#include "DSP.h"
Chorus::Chorus(float lfoFrequency = 1.0f, float modDepth = 1.0f, float lfoOffset = 0.0f)
	: m_lfo{lfoFrequency, lfoOffset, LFOShape::Triangle }, m_modDepthMs(modDepth)
{
	m_baseDelayMs = defaultDelayMs;
}

void Chorus::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
	spec.numChannels = 1;
	m_sampleRate = static_cast<float>(sampleRate);
	m_delayLine.prepare(spec);
	m_lfo.prepareToPlay(m_sampleRate);
	m_maxDelaySamples = std::ceil(msToSamples(chorusMaxDelayMs, m_sampleRate));
	m_minDelaySamples = std::ceil(msToSamples(chorusMinDelayMs, m_sampleRate));
	m_delayLine.setMaximumDelayInSamples(static_cast<int>(m_maxDelaySamples));
}

void Chorus::reset() noexcept
{
	m_delayLine.reset();
	m_lfo.reset();
	setBaseDelaySamples();
	m_delayLine.setDelay(m_baseDelaySamples);
	m_modDepthSamples = std::ceil(msToSamples(m_modDepthMs, m_sampleRate));
}

void Chorus::setBaseDelay(float delayInMs)
{
	if (delayInMs != m_baseDelayMs) {
		m_baseDelayMs = delayInMs;
		setBaseDelaySamples();
	}
}

void Chorus::setBaseDelaySamples() {
	m_baseDelaySamples = msToSamples(m_baseDelayMs, m_sampleRate);
}

float Chorus::processSample(const float& in) {
	float modulatedDelay = m_baseDelaySamples + (m_lfo.process() * m_modDepthSamples);
	modulatedDelay = std::clamp(modulatedDelay, m_minDelaySamples, m_maxDelaySamples);
	m_delayLine.setDelay(modulatedDelay);
	m_delayLine.pushSample(0, in);
	return m_delayLine.popSample(0);
}



