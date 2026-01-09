#include "DelayEngine.h"

DelayEngine::DelayEngine()
{
	m_lowCutFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
	m_highCutFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

void DelayEngine::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = juce::uint32(samplesPerBlock);
	spec.numChannels = 2;
	m_sampleRate = static_cast<float>(sampleRate);
	m_delayLine.prepare(spec);
	m_lowCutFilter.prepare(spec);
	m_highCutFilter.prepare(spec);
	double numSamples = Parameters::maxDelayTime / 1000.0 * m_sampleRate;
	int maxDelayInSamples = int(std::ceil(numSamples));
	m_delayLine.setMaximumDelayInSamples(maxDelayInSamples);
}

void DelayEngine::reset() noexcept
{
	m_delayLine.reset();
	m_lowCutFilter.reset();
	m_highCutFilter.reset();
	m_feedbackL = 0.0f;
	m_feedbackR = 0.0f;
	m_lowCutFreq = -1.0f;
	m_lowCutQ = -1.0f;
	m_highCutFreq = -1.0f;
	m_highCutQ = -1.0f;
	m_delayTimeMs = -1.0f;
	m_mixLevel = 0.5f;
	m_feedbackLevel = 0.0f;
	m_gainLevel = 1.0f;
}

void DelayEngine::setLowCut(const float freq, const float q) {
	if (freq != m_lowCutFreq) {
		m_lowCutFilter.setCutoffFrequency(freq);
		m_lowCutFreq = freq;
	}
	if (q != m_lowCutQ) {
		m_lowCutFilter.setResonance(q);
		m_lowCutQ = q;
	}
}

void DelayEngine::setHighCut(const float freq, const float q) {
	if (freq != m_highCutFreq) {
		m_highCutFilter.setCutoffFrequency(freq);
		m_highCutFreq = freq;
	}
	if (q != m_highCutQ) {
		m_highCutFilter.setResonance(q);
		m_highCutQ = q;
	}
}

void DelayEngine::setDelayTime(const float delayInMs) {
	if (delayInMs != m_delayTimeMs) {
		m_delayTimeMs = delayInMs;
		float delayInSamples = m_delayTimeMs / 1000.0f * m_sampleRate;
		m_delayLine.setDelay(delayInSamples);
	}
}

void DelayEngine::processSample(const float& inL, const float& inR, float& outL, float& outR, const Parameters& params)
{
	setDelayTime(params.delayTime());
	setLowCut(params.lowCutFreq(), params.lowCutQ());
	setHighCut(params.highCutFreq(), params.highCutQ());
	setMixLevel(params.mix());
	setGainLevel(params.gain());
	setFeedbackLevel(params.feedback());

	float dryL = inL;
	float dryR = inR;

	m_delayLine.pushSample(0, dryL + m_feedbackR);
	m_delayLine.pushSample(1, dryR + m_feedbackL);

	float wetL = m_delayLine.popSample(0);
	float wetR = m_delayLine.popSample(1);
	wetL = m_lowCutFilter.processSample(0, wetL);
	wetL = m_highCutFilter.processSample(0, wetL);

	wetR = m_lowCutFilter.processSample(1, wetR);
	wetR = m_highCutFilter.processSample(1, wetR);

	m_feedbackL = wetL * m_feedbackLevel;
	m_feedbackR = wetR * m_feedbackLevel;

	float mixL = dryL * (1.0f - m_mixLevel) + wetL * m_mixLevel;
	float mixR = dryR * (1.0f - m_mixLevel) + wetR * m_mixLevel;

	outL = mixL * m_gainLevel;
	outR = mixR * m_gainLevel;
}