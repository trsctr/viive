#pragma once
#include <JuceHeader.h>
#include "LFO.h"

class Chorus
{
public:
	Chorus(float delayTimeMs = defaultDelayMs, float lfoFrequency = 1.0f, float modDepth = 1.0f, float lfoOffset = 0.0f);
	~Chorus() = default;

	static constexpr float chorusMaxDelayMs = 60.0f;
	static constexpr float chorusMinDelayMs = .64f;
	static constexpr float defaultDelayMs = 7.0f;

	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
	void reset() noexcept;
	void update();
	void kill();
	
	float processSample(const float& in);
	void setBaseDelay(float delayInMs);
	void setModRate(float rate) { m_lfo.setFrequency(rate); }
	void setModDepthMs(float modDepthMs);

private:
	juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> m_delayLine;
	LFO	m_lfo;

	float m_sampleRate = 0.0f;
	float m_coeff = 0.0f;

	float m_baseDelayMs = 0.0f;
	float m_baseDelaySamples = 0.0f;

	float m_maxDelaySamples = 0.0f;
	float m_minDelaySamples = 0.0f;

	float m_modDepthMs = 1.0f;
	float m_modDepthSamples = 1.0f;

	float m_lpState = 0.0f;

	void setBaseDelaySamples();
	void setModDepthSamples();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Chorus)
};