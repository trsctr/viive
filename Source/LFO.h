#pragma once
#include <JuceHeader.h>

enum class LFOShape { Sine };
class LFO {
public:
	LFO();
	~LFO() = default;

	void prepareToPlay(float sampleRate) noexcept;
	void reset() noexcept;
	float process();

private:
	LFOShape m_shape = LFOShape::Sine;

	float m_sampleRate = 0.0f;

	float m_frequency = 1.0f;
	float m_phase = 0.0f;
	float m_offset = 0.0f;
	float m_increment = 0.0f;

	void setIncrement();
	float processSine(float phase);
};