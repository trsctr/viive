#pragma once
#include <JuceHeader.h>

const juce::ParameterID gainParamID{ "gain", 1 };
const juce::ParameterID delayTimeParamID{ "delayTime", 1 };
const juce::ParameterID mixParamID{ "mix", 1 };
const juce::ParameterID feedbackParamID{ "feedback", 1 };
const juce::ParameterID lowCutParamID{ "lowCut", 1 };
const juce::ParameterID highCutParamID{ "highCut", 1 };

class Parameters {
public:
	Parameters(juce::AudioProcessorValueTreeState& apvts);

	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
	static constexpr float minDelayTime = 5.0f;
	static constexpr float maxDelayTime = 5000.0f;

	void prepareToPlay(double sampleRate) noexcept;
	void update() noexcept;
	void reset() noexcept;
	void smoothen() noexcept;

	float gain() const { return m_gain; }
	float delayTime() const { return m_delayTime; }
	float mix() const { return m_mix; }
	float feedback() const { return m_feedback; }
	float lowCut() const { return m_lowCut; }
	float highCut() const { return m_highCut; }

private:
	juce::AudioParameterFloat* m_gainParam;
	juce::AudioParameterFloat* m_delayTimeParam;
	juce::AudioParameterFloat* m_mixParam;
	juce::AudioParameterFloat* m_feedbackParam;
	juce::AudioParameterFloat* m_lowCutParam;
	juce::AudioParameterFloat* m_highCutParam;

	juce::LinearSmoothedValue<float> m_gainSmoother;
	juce::LinearSmoothedValue<float> m_mixSmoother;
	juce::LinearSmoothedValue<float> m_feedbackSmoother;
	juce::LinearSmoothedValue<float> m_lowCutSmoother;
	juce::LinearSmoothedValue<float> m_highCutSmoother;

	float m_targetDelayTime = 0.0f;
	float m_coeff = 0.0f;

	float m_gain = 0.0f;
	float m_delayTime = 0.0f;
	float m_mix = 0.5f;
	float m_feedback = 0.0f;
	float m_lowCut = 20.0f;
	float m_highCut = 20000.0f;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};