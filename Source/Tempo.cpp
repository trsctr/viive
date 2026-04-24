#include "Tempo.h"
#include "Sync.h"

void Tempo::reset() noexcept 
{
	m_bpm = 120;
}

void Tempo::update(const juce::AudioPlayHead* playhead) noexcept
{
	reset();

	if (playhead == nullptr)
		return;

	const auto opt = playhead->getPosition();

	if (!opt.hasValue())
		return;

	const auto& pos = *opt;

	if (pos.getBpm().hasValue())
		m_bpm = *pos.getBpm();

	m_isPlaying = pos.getIsPlaying();

	m_ppqPosition = pos.getPpqPosition().hasValue() ? *pos.getPpqPosition() : 0.0;

}

double Tempo::noteLengthToMs(int index) const noexcept
{
	return 60000.0 * delayNoteLengthMultipliers[size_t(index)] / m_bpm;
}

double Tempo::noteLengthToHz(int index) const noexcept
{
	return m_bpm / (60.0 * modNoteLengthMultipliers[size_t(index)]);
}