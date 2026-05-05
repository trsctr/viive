#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "SyncButton.h"
#include "Selector.h"
#include "LevelMeter.h"
#if JUCE_DEBUG
#include "LFOScope.h"
#endif

//==============================================================================
/**
*/
class ViiveAudioProcessorEditor  :  public juce::AudioProcessorEditor,
    public juce::AudioProcessorValueTreeState::Listener
{
public:
    ViiveAudioProcessorEditor (ViiveAudioProcessor&);
    ~ViiveAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ViiveAudioProcessor& m_audioProcessor;
    LevelMeter m_meter;

    RotaryKnob m_delayTimeLKnob{ "Time L", m_audioProcessor.apvts, delayTimeLParamID.getParamID() };
    RotaryKnob m_delayTimeRKnob{ "Time R", m_audioProcessor.apvts, delayTimeRParamID.getParamID() };
    RotaryKnob m_delayNoteLKnob{ "Note L", m_audioProcessor.apvts, delayNoteLParamID.getParamID() };
    RotaryKnob m_delayNoteRKnob{ "Note R", m_audioProcessor.apvts, delayNoteRParamID.getParamID() };
    
    RotaryKnob m_feedbackKnob{ "Feedback", m_audioProcessor.apvts, feedbackParamID.getParamID() };
    RotaryKnob m_stereoKnob{ "Stereo", m_audioProcessor.apvts, stereoParamID.getParamID() };
    RotaryKnob m_offsetKnob{ "Offset", m_audioProcessor.apvts, offsetParamID.getParamID() };

    RotaryKnob m_lowCutFreqKnob{ "Low Cut Freq", m_audioProcessor.apvts, lowCutFreqParamID.getParamID() };
    RotaryKnob m_lowCutQKnob{ "Low Cut Q", m_audioProcessor.apvts, lowCutQParamID.getParamID() };
    RotaryKnob m_lowCutModRateKnob{ "Rate", m_audioProcessor.apvts, lowCutModRateParamID.getParamID(), RotaryKnob::Size::Small };
    RotaryKnob m_lowCutModNoteKnob{ "Note", m_audioProcessor.apvts, lowCutModNoteParamID.getParamID(), RotaryKnob::Size::Small };
    RotaryKnob m_lowCutModDepthKnob{ "Depth", m_audioProcessor.apvts, lowCutModDepthParamID.getParamID(), RotaryKnob::Size::Small };
    RotaryKnob m_lowCutModPhaseKnob{ "Phase", m_audioProcessor.apvts, lowCutModPhaseParamID.getParamID(), RotaryKnob::Size::Small };
    RotaryKnob m_highCutFreqKnob{ "High Cut Freq", m_audioProcessor.apvts, highCutFreqParamID.getParamID() };
    RotaryKnob m_highCutQKnob{ "High Cut Q", m_audioProcessor.apvts, highCutQParamID.getParamID() };
    RotaryKnob m_highCutModRateKnob{ "Rate", m_audioProcessor.apvts, highCutModRateParamID.getParamID(), RotaryKnob::Size::Small };
    RotaryKnob m_highCutModNoteKnob{ "Note", m_audioProcessor.apvts, highCutModNoteParamID.getParamID(), RotaryKnob::Size::Small };
    RotaryKnob m_highCutModDepthKnob{ "Depth", m_audioProcessor.apvts, highCutModDepthParamID.getParamID(), RotaryKnob::Size::Small };
    RotaryKnob m_highCutModPhaseKnob{ "Phase", m_audioProcessor.apvts, highCutModPhaseParamID.getParamID(), RotaryKnob::Size::Small };

    RotaryKnob m_gainKnob{ "Gain", m_audioProcessor.apvts, gainParamID.getParamID() };
    RotaryKnob m_mixKnob{ "Mix", m_audioProcessor.apvts, mixParamID.getParamID() };

    RotaryKnob m_chorusIntensityKnob{ "Intensity", m_audioProcessor.apvts, chorusIntensityParamID.getParamID() };
	RotaryKnob m_chorusModRateKnob{ "Mod Rate", m_audioProcessor.apvts, chorusModRateParamID.getParamID() };
	RotaryKnob m_chorusModDepthKnob{ "Mod Depth", m_audioProcessor.apvts, chorusModDepthParamID.getParamID() };

    SyncButton m_tempoSyncLButton{ "Sync", m_audioProcessor.apvts, tempoSyncLParamID.getParamID() };
    SyncButton m_tempoSyncRButton{ "Sync", m_audioProcessor.apvts, tempoSyncRParamID.getParamID() };
    SyncButton m_lowCutModTempoSyncButton{ "Sync", m_audioProcessor.apvts, lowCutModTempoSyncParamID.getParamID() };
    SyncButton m_highCutModTempoSyncButton{ "Sync", m_audioProcessor.apvts, highCutModTempoSyncParamID.getParamID() };

    Selector m_modeSelector{ "Mode", m_audioProcessor.apvts, delayModeParamID.getParamID(), Parameters::delayModes, 130 };
    Selector m_lowCutModShapeSelector{ "", m_audioProcessor.apvts, lowCutModShapeParamID.getParamID(), Parameters::lfoShapes, 60, 12.0f };
    Selector m_highCutModShapeSelector{ "", m_audioProcessor.apvts, highCutModShapeParamID.getParamID(), Parameters::lfoShapes, 60, 12.0f };

    juce::Label m_modulationLabel;

    juce::HashMap<juce::String, juce::String> m_linkedParams;

    juce::GroupComponent m_delayGroup, m_filterGroup, m_outputGroup, m_chorusGroup, m_filterModGroup;

#if JUCE_DEBUG
    std::unique_ptr<LFOScope> m_lfoScope;
#endif

    virtual void parameterChanged(const juce::String& paramID, float newValue) override;
    void updateSyncedKnobs(bool syncLActive, bool syncRActive, bool lowCutSyncActive, bool highCutSyncActive);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViiveAudioProcessorEditor)
};
