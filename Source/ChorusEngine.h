#pragma once
#include <JuceHeader.h>
#include "Chorus.h"
#include "Parameters.h"

using Filter = juce::dsp::StateVariableTPTFilter<float>;

class ChorusEngine {
public:
	ChorusEngine();
	~ChorusEngine() = default;

	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
	void reset() noexcept;

	void processSample(const float& inL, const float& inR, float& outL, float& outR, const Parameters& params);
private:
	Chorus m_chorus1L{ 0.2f, 4.0f, 0.0f };
	Chorus m_chorus1R{ 0.2f, 4.f, .5f };
	Chorus m_chorus2L{ 0.45f, 4.f, .5f };
	Chorus m_chorus2R{ 0.45f, 4.f, 0.0f };

	Filter m_highpass;

	float m_coeff = 0.0f;
	float m_dcBlockStateL = 0.0f;
	float m_dcBlockStateR = 0.0f;

	float m_mixLevel = 0.0f;;

	void setMixLevel(float value) { m_mixLevel = value; };
};
