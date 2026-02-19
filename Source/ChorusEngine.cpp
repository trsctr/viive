#include "ChorusEngine.h"
#include "Parameters.h"

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
	m_highpass.setCutoffFrequency(80.0f);
	m_highpass.setResonance(Parameters::defaultFilterQ);
}

void ChorusEngine::processSample(const float& inL, const float& inR, float& outL, float& outR, const Parameters& params)
{
	setMixLevel(params.effectAmt());
	float hpL = m_highpass.processSample(0, inL);
	float hpR = m_highpass.processSample(1, inR);

	float wetL = m_chorus1L.processSample(hpL);
	float wetR = m_chorus1R.processSample(hpR);
	wetL += m_chorus2L.processSample(hpL);
	wetR += m_chorus2R.processSample(hpR);

	outL = inL * (1.0f - m_mixLevel) + wetL * m_mixLevel;
	outR = inR * (1.0f - m_mixLevel) + wetR * m_mixLevel;
}