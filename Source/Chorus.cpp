#include "Chorus.h"
#include "DSP.h"
Chorus::Chorus(float delayTimeMs, float lfoFrequency, float modDepth, float lfoOffset)
	: m_baseDelayMs(delayTimeMs), m_lfo{lfoFrequency, lfoOffset, LFOShape::Triangle }, m_modDepthMs(modDepth)
{
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
	m_coeff = coeffFromFrequency(5000.0f, m_sampleRate);
}

void Chorus::reset() noexcept
{
	m_delayLine.reset();
	m_lfo.reset();
	m_lpState = 0.0f;
	setBaseDelaySamples();
	m_modDepthMs = -1.0f;
}

void Chorus::kill()
{
	m_delayLine.reset();
	m_lpState = 0.0f;
}

void Chorus::setBaseDelay(float delayInMs)
{
	if (!juce::approximatelyEqual(delayInMs, m_baseDelayMs)) {
		m_baseDelayMs = delayInMs;
		setBaseDelaySamples();
	}
}

void Chorus::setBaseDelaySamples() {
	m_baseDelaySamples = msToSamples(m_baseDelayMs, m_sampleRate);
}

void Chorus::setModDepthMs(float modDepthMs)
{
	if (!juce::approximatelyEqual(modDepthMs, m_modDepthMs)) {
		m_modDepthMs = modDepthMs;
		setModDepthSamples();
	}
}

void Chorus::setModDepthSamples()
{
	m_modDepthSamples = std::ceil(msToSamples(m_modDepthMs, m_sampleRate));
}

float Chorus::processSample(const float& in) {
	float modulatedDelay = m_baseDelaySamples + (m_lfo.process() * m_modDepthSamples);
	modulatedDelay = std::clamp(modulatedDelay, m_minDelaySamples, m_maxDelaySamples);
	m_delayLine.setDelay(modulatedDelay);
	m_delayLine.pushSample(0, in);
	float out = m_delayLine.popSample(0);
	return onePoleLowpass(out, m_lpState, m_coeff);
}



