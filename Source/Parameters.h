#pragma once
#include <JuceHeader.h>

const juce::ParameterID gainParamID{ "gain", 1 };
const juce::ParameterID delayTimeParamID{ "delayTime", 1 };
const juce::ParameterID mixParamID{ "mix", 1 };
const juce::ParameterID feedbackParamID{ "feedback", 1 };
const juce::ParameterID stereoParamID{ "stereo", 1 };
const juce::ParameterID effectAmtParamID{ "effectAmt", 1 };
const juce::ParameterID lowCutFreqParamID{ "lowCutFreq", 1 };
const juce::ParameterID lowCutQParamID{ "lowCutQ", 1 };
const juce::ParameterID highCutFreqParamID{ "highCutFreq", 1 };
const juce::ParameterID highCutQParamID{ "highCutQ", 1 };

class Parameters {
public:
	Parameters(juce::AudioProcessorValueTreeState& apvts);

	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
	static constexpr float minDelayTime = 5.0f;
	static constexpr float maxDelayTime = 5000.0f;
	static constexpr float defaultDelayTime = 100.0f;
	static constexpr float maxFilterQ = 2.0f;
	static constexpr float minFilterQ = 0.1f;
	static constexpr float defaultFilterQ = 0.707f;

	void prepareToPlay(double sampleRate) noexcept;
	void update() noexcept;
	void reset() noexcept;
	void smoothen() noexcept;

	float gain() const { return m_gain; }
	float delayTime() const { return m_delayTime; }
	float mix() const { return m_mix; }
	float feedback() const { return m_feedback; }
	float stereo() const { return m_stereo; }
	float effectAmt() const { return m_effectAmt; }
	float lowCutFreq() const { return m_lowCutFreq; }
	float lowCutQ() const { return m_lowCutQ; }
	float highCutFreq() const { return m_highCutFreq; }
	float highCutQ() const { return m_highCutQ; }

private:
	juce::AudioParameterFloat* m_gainParam;
	juce::AudioParameterFloat* m_delayTimeParam;
	juce::AudioParameterFloat* m_mixParam;
	juce::AudioParameterFloat* m_feedbackParam;
	juce::AudioParameterFloat* m_stereoParam;
	juce::AudioParameterFloat* m_effectAmtParam;
	juce::AudioParameterFloat* m_lowCutFreqParam;
	juce::AudioParameterFloat* m_lowCutQParam;
	juce::AudioParameterFloat* m_highCutFreqParam;
	juce::AudioParameterFloat* m_highCutQParam;

	juce::LinearSmoothedValue<float> m_gainSmoother;
	juce::LinearSmoothedValue<float> m_mixSmoother;
	juce::LinearSmoothedValue<float> m_feedbackSmoother;
	juce::LinearSmoothedValue<float> m_stereoSmoother;
	juce::LinearSmoothedValue<float> m_effectAmtSmoother;
	juce::LinearSmoothedValue<float> m_lowCutFreqSmoother;
	juce::LinearSmoothedValue<float> m_lowCutQSmoother;
	juce::LinearSmoothedValue<float> m_highCutFreqSmoother;
	juce::LinearSmoothedValue<float> m_highCutQSmoother;

	float m_targetDelayTime = 0.0f;
	float m_coeff = 0.0f;

	float m_gain = 0.0f;
	float m_delayTime = 0.0f;
	float m_mix = 0.5f;
	float m_feedback = 0.0f;
	float m_stereo = 0.0f;
	float m_effectAmt = 0.0f;
	float m_lowCutFreq = 20.0f;
	float m_lowCutQ = defaultFilterQ;
	float m_highCutFreq = 20000.0f;
	float m_highCutQ = defaultFilterQ;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};