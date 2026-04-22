#include "LFO.h"

LFO::LFO(float frequency, float offset, LFOShape shape)
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
	if (!juce::approximatelyEqual(frequency, m_frequency)) {
		m_frequency = frequency;
		setIncrement();
	}
}

void LFO::setOffset(float offset) {
	m_offset = offset;
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
		case (LFOShape::Square):
			return processSquare(offsetPhase);
		default:
			return 0.0f;
	}
}

float LFO::processSine(float phase) {
	return std::sin(phase * juce::MathConstants<float>::twoPi);
}

float LFO::processTriangle(float phase) {
	return 1.0f - 4.0f * std::abs(phase - 0.5f);
}

float LFO::processSquare(float phase) {
	return (phase < 0.5f) ? 1.0f : -1.0f;
}