#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "ChorusEngine.h"

using Filter = juce::dsp::StateVariableTPTFilter<float>;

class DelayEngine
{
public:
	DelayEngine();
	~DelayEngine() = default;
	
	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
	void reset() noexcept;

	void processSample(const float& inL, const float& inR, float& outL, float& outR, const Parameters& params);
	void setLowCut(const Parameters& params);
	void setHighCut(const Parameters& params);
	void setFilterFreq(const float freq, float& currentFreq, Filter& filter);
	void setFilterQ(const float q, float& currentQ, Filter& filter);
	void setDelayTime(const float delayInMs);
	void setFeedbackLevel(const float value) { m_feedbackLevel = value; }
	void setMixLevel(const float value) { m_mixLevel = value; }
	void setGainLevel(const float value) { m_gainLevel = value; }
	void setWidthLevel(const float value) { m_widthLevel = value; }

private:
	juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> m_delayLine;
	Filter m_lowCutFilter;
	Filter m_highCutFilter;
	Filter m_feedbackHighpass;
	juce::dsp::Compressor<float> m_feedbackCompressor;
	ChorusEngine m_chorusEngine;

	float m_sampleRate = 0.0f;

	float m_feedbackL = 0.0f;
	float m_feedbackR = 0.0f;

	float m_mixLevel = 0.5f;
	float m_feedbackLevel = 0.0f;
	float m_gainLevel = 1.0f;
	float m_widthLevel = 1.0f;
	float m_delayTimeMs = -1.0f;
	float m_lowCutFreq = -1.0f;
	float m_lowCutQ = -1.0f;
	float m_highCutFreq = -1.0f;
	float m_highCutQ = -1.0f;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayEngine)
};