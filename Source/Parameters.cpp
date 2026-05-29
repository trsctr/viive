#include "Parameters.h"
#include "DSP.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
	const juce::ParameterID& id, T& destination)
{
	destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
	jassert(destination);
}

static juce::String stringFromDecibels(float value, int)
{
	return juce::String(value, 2) + " dB";
}

static juce::String stringFromMilliseconds(float value, int)
{
	if (value < 10.0f)
		return juce::String(value, 2) + " ms";
	else if (value < 100.0f)
		return juce::String(value, 1) + " ms";
	else if (value < 1000.0f)
		return juce::String(int(value)) + " ms";
	else
		return juce::String(value * 0.001f, 2) + " s";
}

static juce::String stringFromPercent(float value, int)
{
	return juce::String(int(value)) + " %";
}

static juce::String stringFromHertz(float value, int)
{
	if (value < 20) {
		return juce::String(value, 2) + " Hz";
	}
	if (value < 1000.0f) {
		return juce::String(int(value)) + " Hz";
	}
	else if (value < 10000.0f) {
		return juce::String(value * 0.001f, 1) + " kHz";
	}
	else {
		return juce::String(value * 0.001f, 2) + " kHz";
	}
}

static juce::String stringFromDegrees(float value, int)
{
	return juce::String(int(value)) + "°";
}

static float millisecondsFromString(const juce::String& text)
{
	float value = text.getFloatValue();

	if (!text.endsWithIgnoreCase("ms")) {
		if (text.endsWithIgnoreCase("s") || value < Parameters::minDelayTime) {
			return value * 1000.0f;
		}
	}
	return value;
}

static float hzFromString(const juce::String& text)
{
	float value = text.getFloatValue();
	if (value < 20.0f) {
		return value * 1000.0f;
	}
	return value;
}

static float degreesFromString(const juce::String& text)
{
	return text.getFloatValue();
}

// keep this syncronized with delayNoteLengthMultipliers in Sync.h
const juce::StringArray Parameters::delayNoteLengths = {
	"1/32",
	"1/16 trip",
	"1/32 dot",
	"1/16",
	"1/8 trip",
	"1/16 dot",
	"1/8",
	"1/4 trip",
	"1/8 dot",
	"1/4",
	"1/2 trip",
	"1/4 dot",
	"1/2",
	"1/1 trip",
	"1/2 dot",
	"1/1",
};

// keep this syncronized with modNoteLengthMultipliers in Sync.h
const juce::StringArray Parameters::modNoteLengths = {
	"1/8",
	"1/8 dot",
	"1/4",
	"1/4 dot",
	"1/2",
	"1/2 dot",
	"1/1",
	"1/1 dot",
	"2/1",
	"3/1",
	"4/1",
	"5/1",
	"6/1",
	"7/1",
	"8/1",
};

// keep this syncronized with the DelayMode enum in Types.h
const juce::StringArray Parameters::delayModes = {
	"Stereo",
	"Cross Fb",
	"Ping Pong LR",
	"Ping Pong RL",
};

// keep this syncronized with the LFOShape enum in Types.h
const juce::StringArray Parameters::lfoShapes = {
	"SIN",
	"TRI",
	"SQR",
	"SW+",
	"SW-",
	"S&H",
};

// keep this syncronized with the InsertEffectType enum in Types.h
const juce::StringArray Parameters::insertEffects = {
	"Chorus",
	"Lofi",
};

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
	castParameter(apvts, gainParamID, m_gainParam);
	castParameter(apvts, delayTimeLParamID, m_delayTimeLParam);
	castParameter(apvts, delayTimeRParamID, m_delayTimeRParam);
	castParameter(apvts, delayNoteLParamID, m_delayNoteLParam);
	castParameter(apvts, delayNoteRParamID, m_delayNoteRParam);
	castParameter(apvts, tempoSyncLParamID, m_tempoSyncLParam);
	castParameter(apvts, tempoSyncRParamID, m_tempoSyncRParam);
	castParameter(apvts, delayModeParamID, m_delayModeParam);
	castParameter(apvts, mixParamID, m_mixParam);
	castParameter(apvts, feedbackParamID, m_feedbackParam);
	castParameter(apvts, feedbackKillParamID, m_feedbackKillParam);
	castParameter(apvts, stereoParamID, m_stereoParam);
	castParameter(apvts, offsetParamID, m_offsetParam);
	castParameter(apvts, insertEffectTypeParamID, m_insertEffectTypeParam);
	castParameter(apvts, chorusIntensityParamID, m_chorusIntensityParam);
	castParameter(apvts, chorusModRateParamID, m_chorusModRateParam);
	castParameter(apvts, chorusModDepthParamID, m_chorusModDepthParam);
	castParameter(apvts, lofiResampleFreqParamID, m_lofiResampleFreqParam);
	castParameter(apvts, lofiDampenFreqParamID, m_lofiDampenFreqParam);
	castParameter(apvts, lofiMixLevelParamID, m_lofiMixLevelParam);
	castParameter(apvts, lowCutFreqParamID, m_lowCutFreqParam);
	castParameter(apvts, lowCutQParamID, m_lowCutQParam);
	castParameter(apvts, lowCutModRateParamID, m_lowCutModRateParam);
	castParameter(apvts, lowCutModDepthParamID, m_lowCutModDepthParam);
	castParameter(apvts, lowCutModPhaseParamID, m_lowCutModPhaseParam);
	castParameter(apvts, lowCutModTempoSyncParamID, m_lowCutModTempoSyncParam);
	castParameter(apvts, lowCutModNoteParamID, m_lowCutModNoteParam);
	castParameter(apvts, lowCutModShapeParamID, m_lowCutModShapeParam);
	castParameter(apvts, highCutFreqParamID, m_highCutFreqParam);
	castParameter(apvts, highCutQParamID, m_highCutQParam);
	castParameter(apvts, highCutModRateParamID, m_highCutModRateParam);
	castParameter(apvts, highCutModDepthParamID, m_highCutModDepthParam);
	castParameter(apvts, highCutModPhaseParamID, m_highCutModPhaseParam);
	castParameter(apvts, highCutModTempoSyncParamID, m_highCutModTempoSyncParam);
	castParameter(apvts, highCutModNoteParamID, m_highCutModNoteParam);
	castParameter(apvts, highCutModShapeParamID, m_highCutModShapeParam);
	castParameter(apvts, highCutModInvertParamID, m_highCutModInvertParam);
	castParameter(apvts, lofiNoiseEnabledParamID, m_lofiNoiseEnabledParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
	juce::AudioProcessorValueTreeState::ParameterLayout layout;
	layout.add(std::make_unique<juce::AudioParameterChoice>(
		delayModeParamID.getParamID(),
		"Delay Mode",
		delayModes,
		0
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		delayTimeLParamID.getParamID(),
		"Delay Time L",
		juce::NormalisableRange<float>(minDelayTime, maxDelayTime, 0.001f, 0.25f),
		defaultDelayTime,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromMilliseconds)
			.withValueFromStringFunction(millisecondsFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		delayTimeRParamID.getParamID(),
		"Delay Time R",
		juce::NormalisableRange<float>(minDelayTime, maxDelayTime, 0.001f, 0.25f),
		defaultDelayTime,
		juce::AudioParameterFloatAttributes()
		.withStringFromValueFunction(stringFromMilliseconds)
		.withValueFromStringFunction(millisecondsFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterChoice>(
		delayNoteLParamID,
		"Delay Note L",
		delayNoteLengths,
		9
	));
	layout.add(std::make_unique<juce::AudioParameterChoice>(
		delayNoteRParamID,
		"Delay Note R",
		delayNoteLengths,
		9
	));
	layout.add(std::make_unique<juce::AudioParameterBool>(
		tempoSyncLParamID.getParamID(),
		"Tempo Sync L",
		true
	));
	layout.add(std::make_unique<juce::AudioParameterBool>(
		tempoSyncRParamID.getParamID(),
		"Tempo Sync R",
		true
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		feedbackParamID.getParamID(),
		"Feedback",
		juce::NormalisableRange<float>(0.0f, 140.0f, 1.0f),
		0.0f,
		juce::AudioParameterFloatAttributes()
		.withStringFromValueFunction(stringFromPercent)
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		offsetParamID.getParamID(),
		"Offset",
		juce::NormalisableRange<float>(-50.0f, 50.0f, 0.001f),
		0.0f,
		juce::AudioParameterFloatAttributes()
		.withStringFromValueFunction(stringFromMilliseconds)
		.withValueFromStringFunction(millisecondsFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		mixParamID.getParamID(),
		"Mix",
		juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
		50.0f,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromPercent)
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		stereoParamID.getParamID(),
		"Stereo",
		juce::NormalisableRange<float>(0.0f, 200.0f, 1.0f),
		100.0f,
		juce::AudioParameterFloatAttributes()
		.withStringFromValueFunction(stringFromPercent)
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		gainParamID.getParamID(),
		"Gain",
		juce::NormalisableRange<float>(-12.0f, 6.0f),
		0.0f,
		juce::AudioParameterFloatAttributes()
		.withStringFromValueFunction(stringFromDecibels)
	));
	layout.add(std::make_unique<juce::AudioParameterChoice>(
		insertEffectTypeParamID,
		"Effect Type",
		insertEffects,
		0
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lowCutFreqParamID.getParamID(),
		"Low Cut Frequency",
		juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
		20.0f,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromHertz)
			.withValueFromStringFunction(hzFromString)
		));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lowCutQParamID.getParamID(),
		"Low Cut Q",
		juce::NormalisableRange<float>(minFilterQ, maxFilterQ, 0.01f),
		defaultFilterQ
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lowCutModRateParamID.getParamID(),
		"Low Cut Mod Rate",
		juce::NormalisableRange<float>(0.01f, 8.0f, 0.001f, 0.3f),
		0.05f,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromHertz)
			.withValueFromStringFunction(hzFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterChoice>(
		lowCutModNoteParamID,
		"Low Cut Mod Note",
		modNoteLengths,
		6
	));
	layout.add(std::make_unique<juce::AudioParameterBool>(
		lowCutModTempoSyncParamID.getParamID(),
		"Low Cut Mod Tempo Sync",
		false
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lowCutModDepthParamID.getParamID(),
		"Low Cut Mod Depth",
		juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f),
		0.0f
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lowCutModPhaseParamID.getParamID(),
		"Low Cut Mod Phase",
		juce::NormalisableRange<float>(0.0f, 360.0f, 0.1f),
		0.0f,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromDegrees)
			.withValueFromStringFunction(degreesFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterChoice>(
		lowCutModShapeParamID,
		"Low Cut Mod Shape",
		lfoShapes,
		0
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		highCutFreqParamID.getParamID(),
		"High Cut Frequency",
		juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
		20000.0f,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromHertz)
			.withValueFromStringFunction(hzFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		highCutQParamID.getParamID(),
		"High Cut Q",
		juce::NormalisableRange<float>(minFilterQ, maxFilterQ, 0.01f),
		defaultFilterQ
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		highCutModRateParamID.getParamID(),
		"High Cut Mod Rate",
		juce::NormalisableRange<float>(0.01f, 8.0f, 0.001f, 0.3f),
		0.05f,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromHertz)
			.withValueFromStringFunction(hzFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterChoice>(
		highCutModNoteParamID,
		"High Cut Mod Note",
		modNoteLengths,
		6
	));
	layout.add(std::make_unique<juce::AudioParameterChoice>(
		highCutModShapeParamID,
		"High Cut Mod Shape",
		lfoShapes,
		0
	));
	layout.add(std::make_unique<juce::AudioParameterBool>(
		highCutModTempoSyncParamID.getParamID(),
		"High Cut Mod Tempo Sync",
		false
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		highCutModDepthParamID.getParamID(),
		"High Cut Mod Depth",
		juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f),
		0.0f
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		highCutModPhaseParamID.getParamID(),
		"High Cut Mod Phase",
		juce::NormalisableRange<float>(0.0f, 360.0f, 0.1f),
		0.0f,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromDegrees)
			.withValueFromStringFunction(degreesFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterBool>(
		highCutModInvertParamID.getParamID(),
		"High Cut Mod Invert",
		true
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		chorusIntensityParamID.getParamID(),
		"Chorus Intensity",
		juce::NormalisableRange<float>(0.0f, 1.0f, .01f),
		0.0f
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		chorusModRateParamID.getParamID(),
		"Chorus Mod Rate",
		juce::NormalisableRange<float>(0.01f, 1.0f, 0.01f),
		defaultChorusModRate
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		chorusModDepthParamID.getParamID(),
		"Chorus Mod Depth",
		juce::NormalisableRange<float>(.5f, 6.0f, 0.01f),
		defaultChorusModDepth
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lofiMixLevelParamID.getParamID(),
		"Lofi Mix Level",
		juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
		100.0f,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromPercent)
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lofiResampleFreqParamID.getParamID(),
		"Lofi Resample Freq",
		juce::NormalisableRange<float>(minLofiResampleFreq, maxLofiResampleFreq, 1.0f, .3f),
		maxLofiResampleFreq,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromHertz)
			.withValueFromStringFunction(hzFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterFloat>(
		lofiDampenFreqParamID.getParamID(),
		"Lofi Dampen Freq",
		juce::NormalisableRange<float>(minLofiDampenFreq, maxLofiDampenFreq, 1.0f),
		maxLofiDampenFreq,
		juce::AudioParameterFloatAttributes()
			.withStringFromValueFunction(stringFromHertz)
			.withValueFromStringFunction(hzFromString)
	));
	layout.add(std::make_unique<juce::AudioParameterBool>(
		lofiNoiseEnabledParamID.getParamID(),
		"Lofi Noise",
		true
	));
	layout.add(std::make_unique<juce::AudioParameterBool>(
		feedbackKillParamID.getParamID(),
		"Feedback Kill",
		false
	));
	return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
	double duration = 0.02; // 20 ms smoothing time
	m_gainSmoother.reset(sampleRate, duration);
	m_mixSmoother.reset(sampleRate, duration);
	m_feedbackSmoother.reset(sampleRate, duration);
	m_stereoSmoother.reset(sampleRate, duration);
	m_offsetSmoother.reset(sampleRate, duration);
	m_chorusIntensitySmoother.reset(sampleRate, duration);
	m_lowCutFreqSmoother.reset(sampleRate, duration);
	m_lowCutQSmoother.reset(sampleRate, duration);
	m_lowCutModRateSmoother.reset(sampleRate, duration);
	m_lowCutModDepthSmoother.reset(sampleRate, duration);
	m_lowCutModPhaseSmoother.reset(sampleRate, duration);
	m_highCutFreqSmoother.reset(sampleRate, duration);
	m_highCutQSmoother.reset(sampleRate, duration);
	m_highCutModRateSmoother.reset(sampleRate, duration);
	m_highCutModDepthSmoother.reset(sampleRate, duration);
	m_highCutModPhaseSmoother.reset(sampleRate, duration);
	m_chorusModRateSmoother.reset(sampleRate, duration);
	m_chorusModDepthSmoother.reset(sampleRate, duration);
	m_lofiResampleFreqSmoother.reset(sampleRate, duration);
	m_lofiMixLevelSmoother.reset(sampleRate, duration);
	m_lofiDampenFreqSmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept
{
	m_gain = 0.0f;
	m_delayTimeL = 0.0f;
	m_delayTimeR = 0.0f;
	m_mix = .5f;
	m_feedback = 0.0f;
	m_stereo = 0.0f;
	m_offset = 0.0f;
	m_chorusIntensity = 0.0f;
	m_lowCutFreq = 20.0f;
	m_lowCutQ = defaultFilterQ;
	m_lowCutModRate = 0.0f;
	m_lowCutModDepth = 0.0f;
	m_lowCutModPhase = 0.0f;
	m_highCutFreq = 20000.0f;
	m_highCutQ = defaultFilterQ;
	m_highCutModRate = 0.0f;
	m_highCutModDepth = 0.0f;
	m_highCutModPhase = 0.0f;
	m_chorusModRate = defaultChorusModRate;
	m_chorusModDepth = defaultChorusModDepth;
	m_lofiResampleFreq = maxLofiResampleFreq;
	m_lofiMixLevel = 1.0f;
	m_lofiDampenFreq = maxLofiDampenFreq;
	m_gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(m_gainParam->get()));
	m_mixSmoother.setCurrentAndTargetValue(m_mixParam->get() * 0.01f);
	m_feedbackSmoother.setCurrentAndTargetValue(m_feedbackParam->get() * 0.01f);
	m_stereoSmoother.setCurrentAndTargetValue(m_stereoParam->get() * 0.01f);
	m_chorusIntensitySmoother.setCurrentAndTargetValue(m_chorusIntensityParam->get());
	m_lowCutFreqSmoother.setCurrentAndTargetValue(m_lowCutFreqParam->get());
	m_lowCutQSmoother.setCurrentAndTargetValue(m_lowCutQParam->get());
	m_lowCutModRateSmoother.setCurrentAndTargetValue(m_lowCutModRateParam->get());
	m_lowCutModDepthSmoother.setCurrentAndTargetValue(m_lowCutModDepthParam->get());
	m_lowCutModPhaseSmoother.setCurrentAndTargetValue(m_lowCutModPhaseParam->get());
	m_highCutFreqSmoother.setCurrentAndTargetValue(m_highCutFreqParam->get());
	m_highCutQSmoother.setCurrentAndTargetValue(m_highCutQParam->get());
	m_highCutModRateSmoother.setCurrentAndTargetValue(m_highCutModRateParam->get());
	m_highCutModDepthSmoother.setCurrentAndTargetValue(m_highCutModDepthParam->get());
	m_highCutModPhaseSmoother.setCurrentAndTargetValue(m_highCutModPhaseParam->get());
	m_chorusModRateSmoother.setCurrentAndTargetValue(m_chorusModRateParam->get());
	m_chorusModDepthSmoother.setCurrentAndTargetValue(m_chorusModDepthParam->get());
	m_lofiResampleFreqSmoother.setCurrentAndTargetValue(m_lofiResampleFreqParam->get());
	m_lofiMixLevelSmoother.setCurrentAndTargetValue(m_lofiMixLevelParam->get() * 0.01f);
	m_lofiDampenFreqSmoother.setCurrentAndTargetValue(m_lofiDampenFreqParam->get());
}

void Parameters::update(const Tempo& tempo) noexcept
{
	m_gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(m_gainParam->get()));
	m_mixSmoother.setTargetValue(m_mixParam->get() * 0.01f);
	m_feedbackSmoother.setTargetValue(m_feedbackParam->get() * 0.01f);
	m_stereoSmoother.setTargetValue(m_stereoParam->get() * 0.01f);
	m_offsetSmoother.setTargetValue(m_offsetParam->get());
	m_chorusIntensitySmoother.setTargetValue(m_chorusIntensityParam->get());
	m_lowCutFreqSmoother.setTargetValue(m_lowCutFreqParam->get());
	m_lowCutQSmoother.setTargetValue(m_lowCutQParam->get());
	m_lowCutModRateSmoother.setTargetValue(m_lowCutModRateParam->get());
	m_lowCutModDepthSmoother.setTargetValue(m_lowCutModDepthParam->get());
	m_lowCutModPhaseSmoother.setTargetValue(m_lowCutModPhaseParam->get());
	m_highCutFreqSmoother.setTargetValue(m_highCutFreqParam->get());
	m_highCutQSmoother.setTargetValue(m_highCutQParam->get());
	m_highCutModRateSmoother.setTargetValue(m_highCutModRateParam->get());
	m_highCutModDepthSmoother.setTargetValue(m_highCutModDepthParam->get());
	m_highCutModPhaseSmoother.setTargetValue(m_highCutModPhaseParam->get());
	m_chorusModRateSmoother.setTargetValue(m_chorusModRateParam->get());
	m_chorusModDepthSmoother.setTargetValue(m_chorusModDepthParam->get());
	m_lofiResampleFreqSmoother.setTargetValue(m_lofiResampleFreqParam->get());
	m_lofiMixLevelSmoother.setTargetValue(m_lofiMixLevelParam->get() * 0.01f);
	m_lofiDampenFreqSmoother.setTargetValue(m_lofiDampenFreqParam->get());
	m_lofiNoiseEnabled = m_lofiNoiseEnabledParam->get();
	m_feedbackKill = m_feedbackKillParam->get();
	m_tempoSyncL = m_tempoSyncLParam->get();
	m_tempoSyncR = m_tempoSyncRParam->get();
	m_delayNoteL = m_delayNoteLParam->getIndex();
	m_delayNoteR = m_delayNoteRParam->getIndex();
	m_delayMode = m_delayModeParam->getIndex();
	m_insertEffectType = m_insertEffectTypeParam->getIndex();
	m_lowCutModTempoSync = m_lowCutModTempoSyncParam->get();
	m_lowCutModNote = m_lowCutModNoteParam->getIndex();
	m_highCutModTempoSync = m_highCutModTempoSyncParam->get();
	m_highCutModNote = m_highCutModNoteParam->getIndex();
	m_highCutModInvert = m_highCutModInvertParam->get();
	m_lowCutModShape = m_lowCutModShapeParam->getIndex();
	m_highCutModShape = m_highCutModShapeParam->getIndex();
	if (m_lowCutModTempoSync) {
		float syncedHz = float(tempo.noteLengthToHz(m_lowCutModNote));
	    m_lowCutModRateSmoother.setTargetValue(syncedHz);
	}
	if (m_highCutModTempoSync) {
		float syncedHz = float(tempo.noteLengthToHz(m_highCutModNote));
		m_highCutModRateSmoother.setTargetValue(syncedHz);
	}
	if (m_tempoSyncL) {
		float syncedMs = juce::jmin(float(tempo.noteLengthToMs(m_delayNoteL)), maxDelayTime);
		m_delayTimeLParam->setValueNotifyingHost(m_delayTimeLParam->convertTo0to1(syncedMs));
	}
	if (m_tempoSyncR) {
		float syncedMs = juce::jmin(float(tempo.noteLengthToMs(m_delayNoteR)), maxDelayTime);
		m_delayTimeRParam->setValueNotifyingHost(m_delayTimeRParam->convertTo0to1(syncedMs));
	}
	m_delayTimeL = m_delayTimeLParam->get();
	m_delayTimeR = m_delayTimeRParam->get();
}

void Parameters::smoothen() noexcept
{
	m_gain = m_gainSmoother.getNextValue();
	m_mix = m_mixSmoother.getNextValue();
	m_feedback = m_feedbackSmoother.getNextValue();
	m_stereo = m_stereoSmoother.getNextValue();
	m_offset = m_offsetSmoother.getNextValue();
	m_chorusIntensity = m_chorusIntensitySmoother.getNextValue();
	m_lowCutFreq = m_lowCutFreqSmoother.getNextValue();
	m_lowCutQ = m_lowCutQSmoother.getNextValue();
	m_lowCutModRate = m_lowCutModRateSmoother.getNextValue();
	m_lowCutModDepth = m_lowCutModDepthSmoother.getNextValue();
	m_lowCutModPhase = m_lowCutModPhaseSmoother.getNextValue();
	m_highCutFreq = m_highCutFreqSmoother.getNextValue();
	m_highCutQ = m_highCutQSmoother.getNextValue();
	m_highCutModRate = m_highCutModRateSmoother.getNextValue();
	m_highCutModDepth = m_highCutModDepthSmoother.getNextValue();
	m_highCutModPhase = m_highCutModPhaseSmoother.getNextValue();
	m_chorusModRate = m_chorusModRateSmoother.getNextValue();
	m_chorusModDepth = m_chorusModDepthSmoother.getNextValue();
	m_lofiResampleFreq = m_lofiResampleFreqSmoother.getNextValue();
	m_lofiMixLevel = m_lofiMixLevelSmoother.getNextValue();
	m_lofiDampenFreq = m_lofiDampenFreqSmoother.getNextValue();
}