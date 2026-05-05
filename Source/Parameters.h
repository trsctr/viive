#pragma once
#include <JuceHeader.h>
#include "Tempo.h"

const juce::ParameterID gainParamID{ "gain", 1 };
const juce::ParameterID delayTimeLParamID{ "delayTimeL", 1 };
const juce::ParameterID delayTimeRParamID{ "delayTimeR", 1 };
const juce::ParameterID delayNoteLParamID{ "delayNoteL", 1 };
const juce::ParameterID delayNoteRParamID{ "delayNoteR", 1 };
const juce::ParameterID tempoSyncLParamID{ "tempoSyncL", 1 };
const juce::ParameterID tempoSyncRParamID{ "tempoSyncR", 1 };
const juce::ParameterID delayModeParamID{ "delayMode", 1 };
const juce::ParameterID mixParamID{ "mix", 1 };
const juce::ParameterID feedbackParamID{ "feedback", 1 };
const juce::ParameterID stereoParamID{ "stereo", 1 };
const juce::ParameterID offsetParamID{ "offset", 1 };
const juce::ParameterID lowCutFreqParamID{ "lowCutFreq", 1 };
const juce::ParameterID lowCutQParamID{ "lowCutQ", 1 };
const juce::ParameterID lowCutModRateParamID{ "lowCutModRate", 1 };
const juce::ParameterID lowCutModDepthParamID{ "lowCutModDepth", 1 };
const juce::ParameterID lowCutModPhaseParamID{ "lowCutModPhase", 1 };
const juce::ParameterID lowCutModTempoSyncParamID{ "lowCutModTempoSync", 1 };
const juce::ParameterID lowCutModNoteParamID{ "lowCutModNote", 1 };
const juce::ParameterID lowCutModShapeParamID{ "lowCutModShape", 1 };
const juce::ParameterID highCutFreqParamID{ "highCutFreq", 1 };
const juce::ParameterID highCutQParamID{ "highCutQ", 1 };
const juce::ParameterID highCutModRateParamID{ "highCutModRate", 1 };
const juce::ParameterID highCutModDepthParamID{ "highCutModDepth", 1 };
const juce::ParameterID highCutModPhaseParamID{ "highCutModPhase", 1 };
const juce::ParameterID highCutModTempoSyncParamID{ "highCutModTempoSync", 1 };
const juce::ParameterID highCutModNoteParamID{ "highCutModNote", 1 };
const juce::ParameterID highCutModShapeParamID{ "highCutModShape", 1 };
const juce::ParameterID highCutModInvertParamID{ "highCutModInvert", 1 };
const juce::ParameterID chorusIntensityParamID{ "chorusIntensity", 1 };
const juce::ParameterID chorusModRateParamID{ "chorusModRate", 1 };
const juce::ParameterID chorusModDepthParamID{ "chorusModDepth", 1 };

class Parameters {
public:
	Parameters(juce::AudioProcessorValueTreeState& apvts);

	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
	static constexpr float minDelayTime = 5.0f;
	static constexpr float maxDelayTime = 5000.0f;
	static constexpr float defaultDelayTime = 100.0f;
	static constexpr float minFilterFreq = 20.0f;
	static constexpr float maxFilterFreq = 20000.0f;
	static constexpr float maxFilterQ = 1.5f;
	static constexpr float minFilterQ = 0.1f;
	static constexpr float defaultFilterQ = 0.707f;
	static constexpr float defaultChorusModRate = 0.2f;
	static constexpr float defaultChorusModRate2 = 0.45f; // this is not a passed param but default value for 2nd chorus
	static constexpr float defaultChorusModDepth = 4.0f;
	static const juce::StringArray delayModes;
	static const juce::StringArray delayNoteLengths;
	static const juce::StringArray modNoteLengths;
	static const juce::StringArray lfoShapes;
	
	void prepareToPlay(double sampleRate) noexcept;
	void update(const Tempo& tempo) noexcept;
	void reset() noexcept;
	void smoothen() noexcept;

	float gain() const { return m_gain; }
	float delayTimeL() const { return m_delayTimeL; }
	float delayTimeR() const { return m_delayTimeR; }
	int delayNoteL() const { return m_delayNoteL; }
	int delayNoteR() const { return m_delayNoteR; }
	bool tempoSyncL() const { return m_tempoSyncL; }
	bool tempoSyncR() const { return m_tempoSyncR; }
	int delayMode() const { return m_delayMode; }
	float mix() const { return m_mix; }
	float feedback() const { return m_feedback; }
	float stereo() const { return m_stereo; }
	float offset() const { return m_offset; }
	float lowCutFreq() const { return m_lowCutFreq; }
	float lowCutQ() const { return m_lowCutQ; }
	float lowCutModRate() const { return m_lowCutModRate; }
	float lowCutModDepth() const { return m_lowCutModDepth; }
	float lowCutModPhase() const { return m_lowCutModPhase; }
	bool lowCutModTempoSync() const { return m_lowCutModTempoSync; }
	int lowCutModNote() const { return m_lowCutModNote; }
	int lowCutModShape() const { return m_lowCutModShape; }
	float highCutFreq() const { return m_highCutFreq; }
	float highCutQ() const { return m_highCutQ; }
	float highCutModRate() const { return m_highCutModRate; }
	float highCutModDepth() const { return m_highCutModDepth; }
	float highCutModPhase() const { return m_highCutModPhase; }
	bool highCutModTempoSync() const { return m_highCutModTempoSync; }
	int highCutModNote() const { return m_highCutModNote; }
	int highCutModShape() const { return m_highCutModShape; }
	bool highCutModInvert() const { return m_highCutModInvert; }
	float chorusIntensity() const { return m_chorusIntensity; }
	float chorusModRate() const { return m_chorusModRate; }
	float chorusModDepth() const { return m_chorusModDepth; }

private:
	juce::AudioParameterFloat* m_gainParam;
	juce::AudioParameterFloat* m_delayTimeLParam;
	juce::AudioParameterFloat* m_delayTimeRParam;
	juce::AudioParameterChoice* m_delayNoteLParam;
	juce::AudioParameterChoice* m_delayNoteRParam;
	juce::AudioParameterBool* m_tempoSyncLParam;
	juce::AudioParameterBool* m_tempoSyncRParam;
	juce::AudioParameterChoice* m_delayModeParam;
	juce::AudioParameterFloat* m_mixParam;
	juce::AudioParameterFloat* m_feedbackParam;
	juce::AudioParameterFloat* m_stereoParam;
	juce::AudioParameterFloat* m_offsetParam;
	juce::AudioParameterFloat* m_lowCutFreqParam;
	juce::AudioParameterFloat* m_lowCutQParam;
	juce::AudioParameterFloat* m_lowCutModRateParam;
	juce::AudioParameterFloat* m_lowCutModDepthParam;
	juce::AudioParameterFloat* m_lowCutModPhaseParam;
	juce::AudioParameterBool* m_lowCutModTempoSyncParam;
	juce::AudioParameterChoice* m_lowCutModNoteParam;
	juce::AudioParameterChoice* m_lowCutModShapeParam;
	juce::AudioParameterFloat* m_highCutFreqParam;
	juce::AudioParameterFloat* m_highCutQParam;
	juce::AudioParameterFloat* m_highCutModRateParam;
	juce::AudioParameterFloat* m_highCutModDepthParam;
	juce::AudioParameterFloat* m_highCutModPhaseParam;
	juce::AudioParameterBool* m_highCutModTempoSyncParam;
	juce::AudioParameterChoice* m_highCutModNoteParam;
	juce::AudioParameterChoice* m_highCutModShapeParam;
	juce::AudioParameterBool* m_highCutModInvertParam;
	juce::AudioParameterFloat* m_chorusIntensityParam;
	juce::AudioParameterFloat* m_chorusModRateParam;
	juce::AudioParameterFloat* m_chorusModDepthParam;

	juce::LinearSmoothedValue<float> m_gainSmoother;
	juce::LinearSmoothedValue<float> m_mixSmoother;
	juce::LinearSmoothedValue<float> m_feedbackSmoother;
	juce::LinearSmoothedValue<float> m_stereoSmoother;
	juce::LinearSmoothedValue<float> m_offsetSmoother;
	juce::LinearSmoothedValue<float> m_lowCutFreqSmoother;
	juce::LinearSmoothedValue<float> m_lowCutQSmoother;
	juce::LinearSmoothedValue<float> m_lowCutModRateSmoother;
	juce::LinearSmoothedValue<float> m_lowCutModDepthSmoother;
	juce::LinearSmoothedValue<float> m_lowCutModPhaseSmoother;
	juce::LinearSmoothedValue<float> m_highCutFreqSmoother;
	juce::LinearSmoothedValue<float> m_highCutQSmoother;
	juce::LinearSmoothedValue<float> m_highCutModRateSmoother;
	juce::LinearSmoothedValue<float> m_highCutModDepthSmoother;
	juce::LinearSmoothedValue<float> m_highCutModPhaseSmoother;
	juce::LinearSmoothedValue<float> m_chorusIntensitySmoother;
	juce::LinearSmoothedValue<float> m_chorusModRateSmoother;
	juce::LinearSmoothedValue<float> m_chorusModDepthSmoother;

	float m_targetDelayTimeL = 0.0f;
	float m_targetDelayTimeR = 0.0f;

	float m_coeff = 0.0f;

	float m_gain = 0.0f;
	float m_delayTimeL = 0.0f;
	float m_delayTimeR = 0.0f;
	int m_delayNoteL = 0;
	int m_delayNoteR = 0;
	bool m_tempoSyncL = true;
	bool m_tempoSyncR = true;
	int m_delayMode = 0;
	float m_mix = 0.5f;
	float m_feedback = 0.0f;
	float m_stereo = 0.0f;
	float m_offset = 0.0f;
	float m_lowCutFreq = 20.0f;
	float m_lowCutQ = defaultFilterQ;
	float m_lowCutModRate = 0.0f;
	float m_lowCutModDepth = 0.0f;
	float m_lowCutModPhase = 0.0f;
	bool m_lowCutModTempoSync = false;
	int m_lowCutModNote = 0;
	int m_lowCutModShape = 0;
	float m_highCutFreq = 20000.0f;
	float m_highCutQ = defaultFilterQ;
	float m_highCutModRate = 0.0f;
	float m_highCutModDepth = 0.0f;
	float m_highCutModPhase = 0.0f;
	bool m_highCutModTempoSync = false;
	int m_highCutModNote = 0;
	int m_highCutModShape = 0;
	bool m_highCutModInvert = false;
	float m_chorusIntensity = 0.0f;
	float m_chorusModRate = defaultChorusModRate;
	float m_chorusModDepth = defaultChorusModDepth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};