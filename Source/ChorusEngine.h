#pragma once
#include <JuceHeader.h>
#include "InsertEffect.h"
#include "Chorus.h"
#include "Parameters.h"

using Filter = juce::dsp::StateVariableTPTFilter<float>;

class ChorusEngine : public InsertEffect
{
public:
	ChorusEngine();
	~ChorusEngine() = default;

	static constexpr float chorus1DefaultTime = 7.0f;
	static constexpr float chorus2DefaultTime = 11.0f;


	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept override;
	void reset() noexcept override;
	void kill() override;

	void processSample(const float& inL, const float& inR, float& outL, float& outR) override;
	void update(const Parameters& params) override;

private:
	Chorus m_chorus1L{ chorus1DefaultTime, Parameters::defaultChorusModRate, Parameters::defaultChorusModDepth, 0.0f };
	Chorus m_chorus1R{ chorus1DefaultTime, Parameters::defaultChorusModRate, Parameters::defaultChorusModDepth, .5f };
	Chorus m_chorus2L{ chorus2DefaultTime, Parameters::defaultChorusModRate2, Parameters::defaultChorusModDepth, .5f };
	Chorus m_chorus2R{ chorus2DefaultTime, Parameters::defaultChorusModRate2, Parameters::defaultChorusModDepth, 0.0f };

	Filter m_highpass;

	float m_dcBlockCoeff = 0.0f;
	float m_dcBlockStateL = 0.0f;
	float m_dcBlockStateR = 0.0f;

	float m_intensity = 0.0f;
	float m_multiplier = 1.5f;

	float m_modRate = Parameters::defaultChorusModRate;
	float m_modRateRatio = Parameters::defaultChorusModRate2 / Parameters::defaultChorusModRate;
	float m_modDepth = Parameters::defaultChorusModDepth;

	void setIntensity(float value) { m_intensity = value; }
	void setModDepth(float newModDepth);
	void setModRate(float newRate);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChorusEngine)
};
