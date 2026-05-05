#pragma once
#include <JuceHeader.h>
#include "Types.h"

class LFO {
public:
	LFO(float frequency = 1.0f, float offset = 0.0f, LFOShape shape = LFOShape::Sine);
	~LFO() = default;

	void prepareToPlay(float sampleRate) noexcept;
	void reset() noexcept;
	void setFrequency(float frequency);
	void setOffset(float offset);
	void setShape(LFOShape shape);
	void setPhase(float phase);
	void setSeed(int seed) { m_random.setSeed(seed); }
	float process();

private:
	LFOShape m_shape;
	juce::Random m_random;

	float m_sampleRate = 0.0f;

	float m_frequency = 1.0f;
	float m_phase = 0.0f;
	float m_offset = 0.0f;
	float m_increment = 0.0f;
	float m_lastPhase = 0.0f;
	float m_lastOffsetPhase = 0.0f;
	float m_currentSample = 0.0f;
	float m_nextSample = 0.0f;


	void setIncrement();
	float processSine(float phase);
	float processTriangle(float phase);
	float processSquare(float phase);
	float processSawUp(float phase);
	float processSawDown(float phase);
	float processSampleAndHold(float phase);
};