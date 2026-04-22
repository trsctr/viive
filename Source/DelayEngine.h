#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "ChorusEngine.h"
#include "StereoDelay.h"
#include "FilterEngine.h"
#if JUCE_DEBUG
#include "LFOScopeBuffer.h"
#endif

using Filter = juce::dsp::StateVariableTPTFilter<float>;

enum class DelayMode { Stereo, Cross, PingPongLR, PingPongRL};

class DelayEngine
{
public:
	DelayEngine();
	~DelayEngine() = default;
	
	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
	void reset() noexcept;

	void update(const Parameters& params);
	void processSample(const float& inL, const float& inR, float& outL, float& outR, const Parameters& params);
	void setDelayTimes(const float delayInMsL, const float delayInMsR);
	void setFeedbackLevel(const float value) { m_feedbackLevel = value; }
	void setMixLevel(const float value) { m_mixLevel = value; }
	void setGainLevel(const float value) { m_gainLevel = value; }
	void setWidthLevel(const float value) { m_widthLevel = value; }
	void setOffsetMs(const float value) { m_offsetMs = value * .5f; }
	void setDelayMode(const int modeIndex);

#if JUCE_DEBUG
	LFOScopeBuffer& getLFOScopeBuffer() { return m_filterEngine.m_scopeBuffer; }
#endif

private:
	StereoDelay m_stereoDelay;
	DelayMode m_delayMode = DelayMode::Stereo;
	FilterEngine m_filterEngine;
	Filter m_feedbackHighpass;
	juce::dsp::Compressor<float> m_feedbackCompressor;
	ChorusEngine m_chorusEngine;
	
	float m_sampleRate = 0.0f;
	
	float m_coeff = 0.0f;

	float m_feedbackL = 0.0f;
	float m_feedbackR = 0.0f;

	float m_baseDelayTimeMs = -1.0f;
	float m_delayTimeMsL = -1.0f;
	float m_delayTimeMsR = -1.0f;

	float m_mixLevel = 0.5f;
	float m_feedbackLevel = 0.0f;
	float m_gainLevel = 1.0f;
	float m_widthLevel = 1.0f;
	float m_offsetMs = 0.0f;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayEngine)
};