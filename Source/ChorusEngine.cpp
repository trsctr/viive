#include "ChorusEngine.h"
#include "Parameters.h"
#include "DSP.h"

ChorusEngine::ChorusEngine()
{
	m_highpass.setType(juce::dsp::StateVariableTPTFilterType::highpass);
}

void ChorusEngine::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = juce::uint32(samplesPerBlock);
	spec.numChannels = 2;
	m_highpass.prepare(spec);
	m_dcBlockCoeff = coeffFromFrequency(5.0f, static_cast<float>(sampleRate));
	m_chorus1L.prepareToPlay(sampleRate, samplesPerBlock);
	m_chorus1R.prepareToPlay(sampleRate, samplesPerBlock);
	m_chorus2L.prepareToPlay(sampleRate, samplesPerBlock);
	m_chorus2R.prepareToPlay(sampleRate, samplesPerBlock);
}

void ChorusEngine::reset() noexcept
{
	m_chorus1L.reset();
	m_chorus1R.reset();
	m_chorus2L.reset();
	m_chorus2R.reset();
	m_highpass.reset();
	m_dcBlockStateL = 0.0f;
	m_dcBlockStateR = 0.0f;
	m_highpass.setCutoffFrequency(100.0f);
	m_highpass.setResonance(Parameters::defaultFilterQ);
	m_modRate = -1.0f;
	m_modDepth = -1.0f;
}

void ChorusEngine::setModRate(float newRate)
{
	if (!juce::approximatelyEqual(newRate, m_modRate)) {
		m_modRate = newRate;
		m_chorus1L.setModRate(m_modRate);
		m_chorus1R.setModRate(m_modRate);
		m_chorus2L.setModRate(m_modRate * m_modRateRatio);
		m_chorus2R.setModRate(m_modRate * m_modRateRatio);
	}
}

void ChorusEngine::setModDepth(float modDepth)
{
	if (!juce::approximatelyEqual(modDepth, m_modDepth))
	{
		m_modDepth = modDepth;
		m_chorus1L.setModDepthMs(m_modDepth);
		m_chorus1R.setModDepthMs(m_modDepth);
		m_chorus2L.setModDepthMs(m_modDepth);
		m_chorus2R.setModDepthMs(m_modDepth);
	}
}

void ChorusEngine::processSample(const float& inL, const float& inR, float& outL, float& outR, const Parameters& params)
{
	setIntensity(params.chorusIntensity());
	setModRate(params.chorusModRate());
	setModDepth(params.chorusModDepth());

	// Dual chorus intensity mapping: c1 peaks early (at ~0.6 intensity),
	// c2 ramps exponentially, staying subtle until higher intensities.
	// This creates a smooth character progression: barely audible -> chorus1 dominant
	// -> gradual blend with chorus2 at full intensity.
	float c1Intensity = std::min(m_intensity * 1.667f, 1.0f);
	float c2Intensity = std::pow(m_intensity, 2.5f);

	float hpL = m_highpass.processSample(0, inL);
	float hpR = m_highpass.processSample(1, inR);

	float chorus1L = m_chorus1L.processSample(hpL);
	float chorus1R = m_chorus1R.processSample(hpR);
	
	// dcBlocker using one pole highpass
	chorus1L -= onePoleLowpass(chorus1L, m_dcBlockStateL, m_dcBlockCoeff);
	chorus1R -= onePoleLowpass(chorus1R, m_dcBlockStateR, m_dcBlockCoeff);

	float chorus2L = m_chorus2L.processSample(hpL);
	float chorus2R= m_chorus2R.processSample(hpR);
	
	chorus2L -= onePoleLowpass(chorus2L, m_dcBlockStateL, m_dcBlockCoeff);
	chorus2R -= onePoleLowpass(chorus2R, m_dcBlockStateR, m_dcBlockCoeff);
	
	
	// Blend both chorus voices according to their intensity curves
	float wetL = (chorus1L * c1Intensity) + (chorus2L * c2Intensity);
	float wetR = (chorus1R * c1Intensity) + (chorus2R * c2Intensity);

	float wetIntensity = (c1Intensity + c2Intensity) *.5f;

	// Additive mix with preserved dry signal prevents feedback spiral in the delay loop.
	// m_multiplier controls how loud the wet chorus signal is relative to dry signal.
	// Makeup gain normalizes output to prevent clipping.

	outL = inL + (wetL * m_multiplier);
	outR = inR + (wetR * m_multiplier);
	float makeupGain = 1.0f / (1.0f + wetIntensity * m_multiplier);

	outL *= makeupGain;
	outR *= makeupGain;
}