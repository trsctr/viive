#include "DelayEngine.h"
#include "DSP.h"
DelayEngine::DelayEngine()
{
	m_lowCutFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
	m_highCutFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
	m_feedbackHighpass.setType(juce::dsp::StateVariableTPTFilterType::highpass);
	m_feedbackCompressor.setAttack(.1f);
	m_feedbackCompressor.setRelease(50.f);
	m_feedbackCompressor.setRatio(8.0f);
	m_feedbackCompressor.setThreshold(-6.0f);
}

void DelayEngine::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = juce::uint32(samplesPerBlock);
	spec.numChannels = 2;
	m_sampleRate = static_cast<float>(sampleRate);
	m_feedbackCompressor.prepare(spec);
	m_lowCutFilter.prepare(spec);
	m_highCutFilter.prepare(spec);
	m_feedbackHighpass.prepare(spec);
	m_chorusEngine.prepareToPlay(sampleRate, samplesPerBlock);
	m_stereoDelay.prepareToPlay(sampleRate, samplesPerBlock);
	m_coeff = onePoleLowpassCoeff(100.0f, static_cast<float>(sampleRate));

}

void DelayEngine::reset() noexcept
{
	m_stereoDelay.reset();
	m_lowCutFilter.reset();
	m_highCutFilter.reset();
	m_chorusEngine.reset();
	m_feedbackHighpass.reset();
	m_feedbackHighpass.setCutoffFrequency(60.0f); // just to remove rumble from fb loop
	m_feedbackHighpass.setResonance(Parameters::defaultFilterQ);
	m_feedbackCompressor.reset();
	m_feedbackL = 0.0f;
	m_feedbackR = 0.0f;
	m_lowCutFreq = -1.0f;
	m_lowCutQ = -1.0f;
	m_highCutFreq = -1.0f;
	m_highCutQ = -1.0f;
	m_baseDelayTimeMs = -1.0f;
	m_mixLevel = 0.5f;
	m_feedbackLevel = 0.0f;
	m_gainLevel = 1.0f;
}

void DelayEngine::setDelayMode(const int modeIndex) 
{
	DelayMode newMode = static_cast<DelayMode>(modeIndex);
	if (newMode != m_delayMode) {
		m_delayMode = newMode;
	}
}

void DelayEngine::setLowCut(const Parameters& params) {
	setFilterFreq(params.lowCutFreq(), m_lowCutFreq, m_lowCutFilter);
	setFilterQ(params.lowCutQ(), m_lowCutQ, m_lowCutFilter);
}

void DelayEngine::setHighCut(const Parameters& params) {
	setFilterFreq(params.highCutFreq(), m_highCutFreq, m_highCutFilter);
	setFilterQ(params.highCutQ(), m_highCutQ, m_highCutFilter);
}

void DelayEngine::setFilterFreq(const float freq, float& currentFreq, Filter& filter) {
	if (freq != currentFreq) {
		filter.setCutoffFrequency(freq);
		currentFreq = freq;
	}
}

void DelayEngine::setFilterQ(const float q, float& currentQ, Filter& filter) {
	float newQ = std::min(q, Parameters::maxFilterQ);
	if (newQ != currentQ) {
		filter.setResonance(newQ);
		currentQ = newQ;
	}
}

void DelayEngine::setDelayTimes(const float targetL, const float targetR) {
	float targetLeftMs = targetL - m_offsetMs;
	float targetRightMs = targetR + m_offsetMs;
	if (m_delayTimeMsL == 0.0f) m_delayTimeMsL = targetLeftMs;
	if (m_delayTimeMsR == 0.0f) m_delayTimeMsR = targetRightMs;
	m_delayTimeMsL = onePoleLowpass(targetLeftMs, m_delayTimeMsL, m_coeff);
	m_delayTimeMsR = onePoleLowpass(targetRightMs, m_delayTimeMsR, m_coeff);
	float leftMs = juce::jlimit(Parameters::minDelayTime, Parameters::maxDelayTime, m_delayTimeMsL);
	float rightMs = juce::jlimit(Parameters::minDelayTime, Parameters::maxDelayTime, m_delayTimeMsR);
	m_stereoDelay.setDelayTime(leftMs, Channel::Left);
	m_stereoDelay.setDelayTime(rightMs, Channel::Right);
}

void DelayEngine::update(const Parameters& params)
{
	setLowCut(params);
	setHighCut(params);
	setMixLevel(params.mix());
	setGainLevel(params.gain());
	setFeedbackLevel(params.feedback());
	setWidthLevel(params.stereo());
	setOffsetMs(params.offset());
	setDelayTimes(params.delayTimeL(), params.delayTimeR());
}

void DelayEngine::processSample(const float& inL, const float& inR, float& outL, float& outR, const Parameters& params)
{
	float dryL = inL;
	float dryR = inR;

	float mono = (dryL + dryR) * .5;
	float delayInL = 0.0f, delayInR = 0.0f, popL = 0.0f, popR = 0.0f, wetL = 0.0f, wetR = 0.0f;
	switch (m_delayMode) {
		case DelayMode::PingPongLR:
			delayInL = mono + m_feedbackR;
			delayInR = m_feedbackL;
			//DBG("Mode: PingPongL");
			break;
		case DelayMode::PingPongRL:
			delayInL = m_feedbackR;
			delayInR = mono + m_feedbackL;
			//DBG("Mode: PingPongR");
			break;
		case DelayMode::Cross: 
			delayInL = dryL + m_feedbackR;
			delayInR = dryR + m_feedbackL;
			//DBG("Mode: Cross");
			break;
		case DelayMode::Stereo:
			delayInL = dryL + m_feedbackL;
			delayInR = dryR + m_feedbackR;
		//	DBG("Mode: Stereo");
			break;
		default:
			delayInL = dryL;
			delayInR = dryR;
	//		DBG("Falling through for default");
	}

	m_stereoDelay.processSample(delayInL, popL, Channel::Left);
	m_stereoDelay.processSample(delayInR, popR, Channel::Right);
	
	m_chorusEngine.processSample(popL, popR, wetL, wetR, params);

	// sculpting filters, first left and then right channel
	wetL = m_lowCutFilter.processSample(0, wetL);
	wetL = m_highCutFilter.processSample(0, wetL);

	wetR = m_lowCutFilter.processSample(1, wetR);
	wetR = m_highCutFilter.processSample(1, wetR);

	// this removes rumble generated by low cut filter on higher q values
	wetL = m_feedbackHighpass.processSample(0, wetL); 
	wetR = m_feedbackHighpass.processSample(1, wetR);

	// compress the high passed signal with aggressive settings to limit
	// the output level and avoid overloading the fb
	m_feedbackL = m_feedbackCompressor.processSample(0, wetL * m_feedbackLevel);
	m_feedbackR = m_feedbackCompressor.processSample(1, wetR * m_feedbackLevel);

	// soft clipping to smoothen things and to ensure it doesnt overload
	m_feedbackL = std::tanh(m_feedbackL);
	m_feedbackR = std::tanh(m_feedbackR);

	float mid = (wetL + wetR) * .5;
	float side = (wetL - wetR) * .5;

	side *= m_widthLevel;

	wetL = mid + side;
	wetR = mid - side;

	//autopan could be here

	float mixL = dryL * (1.0f - m_mixLevel) + wetL * m_mixLevel;
	float mixR = dryR * (1.0f - m_mixLevel) + wetR * m_mixLevel;

	outL = mixL * m_gainLevel;
	outR = mixR * m_gainLevel;
}