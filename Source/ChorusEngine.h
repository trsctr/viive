#pragma once
#include <JuceHeader.h>
#include "Chorus.h"
#include "Parameters.h"

using Filter = juce::dsp::StateVariableTPTFilter<float>;

class ChorusEngine {
public:
	ChorusEngine();
	~ChorusEngine() = default;

	static constexpr float chorus1DefaultTime = 7.0f;
	static constexpr float chorus2DefaultTime = 11.0f;
	static constexpr float chorus1DefaultFreq = 0.2f;
	static constexpr float chorus2DefaultFreq = 0.45f;
	static constexpr float defaultModDepth = 4.0f;


	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
	void reset() noexcept;

	void processSample(const float& inL, const float& inR, float& outL, float& outR, const Parameters& params);

private:
	Chorus m_chorus1L{ chorus1DefaultTime, chorus1DefaultFreq, defaultModDepth, 0.0f }; // delay time, lfo frequency, mod depth, phase offset
	Chorus m_chorus1R{ chorus1DefaultTime, chorus1DefaultFreq, defaultModDepth, .5f };
	Chorus m_chorus2L{ chorus2DefaultTime, chorus2DefaultFreq, defaultModDepth, .5f };
	Chorus m_chorus2R{ chorus2DefaultTime, chorus2DefaultFreq, defaultModDepth, 0.0f };

	Filter m_highpass;

	float m_dcBlockCoeff = 0.0f;
	float m_dcBlockStateL = 0.0f;
	float m_dcBlockStateR = 0.0f;

	float m_effectAmt = 0.0f;
	float m_lfoFreq = chorus1DefaultFreq;
	float m_lfoFreqRatio = chorus2DefaultFreq / chorus1DefaultFreq;
	float m_modDepth = defaultModDepth;

	void setEffectAmt(float value) { m_effectAmt = value; }
	void setModDepth(float modDepthMs);
	void setLfoFreq(float lfoFreq);
};
