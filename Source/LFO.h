#pragma once
#include <JuceHeader.h>

enum class LFOShape { Sine, Triangle };
class LFO {
public:
	LFO(float frequency = 1.0f, float offset = 0.0f, LFOShape shape = LFOShape::Sine);
	~LFO() = default;

	void prepareToPlay(float sampleRate) noexcept;
	void reset() noexcept;
	void setFrequency(float frequency);
	void setOffset(float offset);
	void setShape(LFOShape shape);
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
	float processTriangle(float phase);
};