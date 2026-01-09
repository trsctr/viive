#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
	const juce::ParameterID& id, T& destination)
{
	destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
	jassert(destination);
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
	castParameter(apvts, gainParamID, m_gainParam);
	castParameter(apvts, delayTimeParamID, m_delayTimeParam);
	castParameter(apvts, mixParamID, m_mixParam);
	castParameter(apvts, feedbackParamID, m_feedbackParam);
	castParameter(apvts, lowCutFreqParamID, m_lowCutFreqParam);
	castParameter(apvts, lowCutQParamID, m_lowCutQParam);
	castParameter(apvts, highCutFreqParamID, m_highCutFreqParam);
	castParameter(apvts, highCutQParamID, m_highCutQParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
	juce::AudioProcessorValueTreeState::ParameterLayout layout;
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		gainParamID.getParamID(),
		"Gain",
		juce::NormalisableRange<float>(-12.0f, 12.0f),
		0.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		delayTimeParamID.getParamID(),
		"Delay Time",
		juce::NormalisableRange<float>(minDelayTime, maxDelayTime, 0.001f, 0.25f),
		defaultDelayTime));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		mixParamID.getParamID(),
		"Mix",
		juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
		50.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		feedbackParamID.getParamID(),
		"Feedback",
		juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f),
		0.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lowCutFreqParamID.getParamID(),
		"Low Cut Frequency",
		juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
		20.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lowCutQParamID.getParamID(),
		"Low Cut Q",
		juce::NormalisableRange<float>(minFilterQ, maxFilterQ, 0.1f),
		defaultFilterQ));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		highCutFreqParamID.getParamID(),
		"High Cut Frequency",
		juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
		20000.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		highCutQParamID.getParamID(),
		"High Cut Q",
		juce::NormalisableRange<float>(minFilterQ, maxFilterQ, 0.1f),
		defaultFilterQ));
	return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
	double duration = 0.02; // 20 ms smoothing time
	m_gainSmoother.reset(sampleRate, duration);
	m_mixSmoother.reset(sampleRate, duration);
	m_feedbackSmoother.reset(sampleRate, duration);
	m_lowCutFreqSmoother.reset(sampleRate, duration);
	m_lowCutQSmoother.reset(sampleRate, duration);
	m_highCutFreqSmoother.reset(sampleRate, duration);
	m_highCutQSmoother.reset(sampleRate, duration);
	m_coeff = 1.0f - std::exp(-1.0f / (0.1f * static_cast<float>(sampleRate)));
}

void Parameters::reset() noexcept
{
	m_gain = 0.0f;
	m_delayTime = 0.0f;
	m_mix = .5f;
	m_feedback = 0.0f;
	m_lowCutFreq = 20.0f;
	m_lowCutQ = defaultFilterQ;
	m_highCutFreq = 20000.0f;
	m_highCutQ = defaultFilterQ;
	m_gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(m_gainParam->get()));
	m_mixSmoother.setCurrentAndTargetValue(m_mixParam->get() * 0.01f);
	m_feedbackSmoother.setCurrentAndTargetValue(m_feedbackParam->get() * 0.01f);
	m_lowCutFreqSmoother.setCurrentAndTargetValue(m_lowCutFreqParam->get());
	m_lowCutQSmoother.setCurrentAndTargetValue(m_lowCutQParam->get());
	m_highCutFreqSmoother.setCurrentAndTargetValue(m_highCutFreqParam->get());
	m_highCutQSmoother.setCurrentAndTargetValue(m_highCutQParam->get());
}

void Parameters::update() noexcept
{
	m_gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(m_gainParam->get()));
	m_mixSmoother.setTargetValue(m_mixParam->get() * 0.01f);
	m_feedbackSmoother.setTargetValue(m_feedbackParam->get() * 0.01f);
	m_lowCutFreqSmoother.setTargetValue(m_lowCutFreqParam->get());
	m_lowCutQSmoother.setTargetValue(m_lowCutQParam->get());
	m_highCutFreqSmoother.setTargetValue(m_highCutFreqParam->get());
	m_highCutQSmoother.setTargetValue(m_highCutQParam->get());
	m_targetDelayTime = m_delayTimeParam->get();
	if (m_delayTime == 0.0f) {
		m_delayTime = m_targetDelayTime;
	}
}

void Parameters::smoothen() noexcept
{
	m_gain = m_gainSmoother.getNextValue();
	m_mix = m_mixSmoother.getNextValue();
	m_feedback = m_feedbackSmoother.getNextValue();
	m_lowCutFreq = m_lowCutFreqSmoother.getNextValue();
	m_lowCutQ = m_lowCutQSmoother.getNextValue();
	m_highCutFreq = m_highCutFreqSmoother.getNextValue();
	m_highCutQ = m_highCutQSmoother.getNextValue();
	m_delayTime += (m_targetDelayTime - m_delayTime) * m_coeff;
}