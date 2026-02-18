#include "LFO.h"

LFO::LFO(float frequency = 1.0f, float offset = 0.0f, LFOShape shape = LFOShape::Sine)
	: m_frequency(frequency), m_offset(offset), m_shape(shape)
{
}

void LFO::prepareToPlay(float sampleRate) noexcept {
	m_sampleRate = sampleRate;
}

void LFO::reset() noexcept {
	m_phase = 0.0f;
	setIncrement();
}

void LFO::setFrequency(float frequency) {
	if (frequency != m_frequency) {
		m_frequency = frequency;
		setIncrement();
	}
}

void LFO::setOffset(float offset) {
	m_offset = offset;
	DBG(m_offset);
}

void LFO::setShape(LFOShape shape) {
	m_shape = shape;
}

void LFO::setIncrement() {
	if (m_sampleRate > 0.0f) {
		m_increment = m_frequency / m_sampleRate;
	}
	else
		m_increment = 0.0f;
}

float LFO::process() {
	m_phase += m_increment;
	m_phase = std::fmod(m_phase, 1.0f);
	float offsetPhase = std::fmod(m_phase + m_offset, 1.0f);
	switch (m_shape) {
		case (LFOShape::Sine):
			return processSine(offsetPhase);
		case (LFOShape::Triangle):
			return processTriangle(offsetPhase);
	}
}

float LFO::processSine(float phase) {
	return std::sin(phase * juce::MathConstants<float>::twoPi);
}

float LFO::processTriangle(float phase) {
	return 1.0f - 4.0f * std::abs(phase - 0.5f);
}