#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class DelayEngine
{
public:
	DelayEngine();
	~DelayEngine() = default;
	
	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
	void reset() noexcept;

	void processSample(const float& inL, const float& inR, float& outL, float& outR, const Parameters& params);
	void setLowCutFreq(const float freq);
	void setHighCutFreq(const float freq);
	void setDelayTime(const float delayInMs);
	void setFeedbackLevel(const float value) { m_feedbackLevel = value; }
	void setMixLevel(const float value) { m_mixLevel = value; }
	void setGainLevel(const float value) { m_gainLevel = value; }

private:
	juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> m_delayLine;
	juce::dsp::StateVariableTPTFilter<float> m_lowCutFilter;
	juce::dsp::StateVariableTPTFilter<float> m_highCutFilter;

	float m_sampleRate = 0.0f;

	float m_feedbackL = 0.0f;
	float m_feedbackR = 0.0f;

	float m_mixLevel = 0.5f;
	float m_feedbackLevel = 0.0f;
	float m_gainLevel = 1.0f;
	float m_delayTimeMs = -1.0f;
	float m_lowCutFreq = -1.0f;
	float m_highCutFreq = -1.0f;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayEngine)
};