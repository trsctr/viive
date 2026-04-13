#pragma once
#include <JuceHeader.h>

enum Channel { Left = 0, Right = 1 };

class StereoDelay {
public:
	StereoDelay();
	~StereoDelay() = default;

	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
	void reset() noexcept;
	void processSample(const float &in, float &out, Channel channel);

	void setDelayTime(float ms, Channel channel);

private:
	juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> m_delayLines[2];

	float m_delayTimesMs[2] = { 0.0f, 0.0f };
	float m_delayTimesSamples[2] = { 0.0f, 0.0f };
	float m_sampleRate = 0.0f;

	void setDelayTimeSamples(float samples, Channel channel);
};