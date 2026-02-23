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
	if (newRate != m_modRate) {
		m_modRate = newRate;
		m_chorus1L.setModRate(m_modRate);
		m_chorus1R.setModRate(m_modRate);
		m_chorus2L.setModRate(m_modRate * m_modRateRatio);
		m_chorus2R.setModRate(m_modRate * m_modRateRatio);
	}
}

void ChorusEngine::setModDepth(float modDepth)
{
	if (modDepth != m_modDepth)
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

	float hpL = m_highpass.processSample(0, inL);
	float hpR = m_highpass.processSample(1, inR);

	float wetL = m_chorus1L.processSample(hpL);
	float wetR = m_chorus1R.processSample(hpR);
	
	// dc block with onePoleHighpass
	wetL -= onePoleLowpass(wetL, m_dcBlockStateL, m_dcBlockCoeff);
	wetR -= onePoleLowpass(wetR, m_dcBlockStateR, m_dcBlockCoeff);

	wetL += m_chorus2L.processSample(hpL);
	wetR += m_chorus2R.processSample(hpR);
	
	wetL -= onePoleLowpass(wetL, m_dcBlockStateL, m_dcBlockCoeff);
	wetR -= onePoleLowpass(wetR, m_dcBlockStateR, m_dcBlockCoeff);
	
	// using additive mixing to blend chorused signal with the dry signal
	// to make sure feedback does not spiral out of control when chorus
	// intensity is full.
	// 2.0f multiplier seems to be a nice sweetspot
	// so the modulated signal is louder than dry signal
	// so we get some of that sweet chorus action

	outL = inL + (wetL * m_intensity * m_multiplier);
	outR = inR + (wetR * m_intensity * m_multiplier);
	float makeupGain = 1.0f / (1.0f + m_intensity * m_multiplier);

	outL *= makeupGain;
	outR *= makeupGain;
}