#pragma once
#include <JuceHeader.h>

class Chorus
{
public:
	Chorus();
	~Chorus() = default;

	static constexpr float chorusMaxDelayMs = 60.0f;
	static constexpr float chorusMinDelayMs = 5.0f;


	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
	void reset() noexcept;
	void update();
	float processSample(const float& in);

	void setBaseDelayMs(float baseDelayMs);
	void setBaseDelaySamples();

private:
	juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> m_delayLine;

	float m_sampleRate = 0.0f;

	float m_baseDelayMs = 0.0f;
	float m_baseDelaySamples = 0.0f;

	float m_maxDelaySamples = 0.0f;
	float m_minDelaySamples = 0.0f;

	float m_modDepthMs = 1.0f;
	float m_modDepthSamples = 1.0f;

	float m_lfoFrequency = 1.0f;
	float m_lfoPhase = 0.0f;
	float m_lfoOffset = 0.0f;
	float m_lfoIncrement = 0.0f;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Chorus);
};