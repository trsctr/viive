#pragma once

#include <JuceHeader.h>

class Tempo
{
public:
	void reset() noexcept;
	
	void update(const juce::AudioPlayHead* playhead) noexcept;

	double noteLengthToMs(int index) const noexcept;

	double getTempo() const noexcept { return m_bpm; }

private:
	double m_bpm = 120.0;
};