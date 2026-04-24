#pragma once

#include <JuceHeader.h>

class Tempo
{
public:
	void reset() noexcept;
	
	void update(const juce::AudioPlayHead* playhead) noexcept;

	double noteLengthToMs(int index) const noexcept;
	double noteLengthToHz(int index) const noexcept;

	double getTempo() const noexcept { return m_bpm; }
	bool getIsPlaying() const noexcept { return m_isPlaying; }
	double getPpqPosition() const noexcept { return m_ppqPosition; }

private:
	double m_bpm = 120.0;
	bool m_isPlaying = false;
	double m_ppqPosition = 0.0;
};