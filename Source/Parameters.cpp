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
	castParameter(apvts, lowCutParamID, m_lowCutParam);
	castParameter(apvts, highCutParamID, m_highCutParam);
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
		100.0f));
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
		lowCutParamID.getParamID(),
		"Low Cut",
		juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
		20.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		highCutParamID.getParamID(),
		"High Cut",
		juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
		20000.0f));
	return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
	double duration = 0.02; // 20 ms smoothing time
	m_gainSmoother.reset(sampleRate, duration);
	m_mixSmoother.reset(sampleRate, duration);
	m_feedbackSmoother.reset(sampleRate, duration);
	m_lowCutSmoother.reset(sampleRate, duration);
	m_highCutSmoother.reset(sampleRate, duration);
	m_coeff = 1.0f - std::exp(-1.0f / (0.1f * static_cast<float>(sampleRate)));
}

