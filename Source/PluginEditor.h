#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "Rotaryknob.h"
#include "LevelMeter.h"

//==============================================================================
/**
*/
class ViiveAudioProcessorEditor  : public juce::AudioProcessorEditor
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

    RotaryKnob m_delayTimeKnob{ "Time", m_audioProcessor.apvts, delayTimeParamID.getParamID() };
    RotaryKnob m_feedbackKnob{ "Feedback", m_audioProcessor.apvts, feedbackParamID.getParamID() };
    RotaryKnob m_stereoKnob{ "Stereo", m_audioProcessor.apvts, stereoParamID.getParamID() };

    RotaryKnob m_lowCutFreqKnob{ "Low Cut Freq", m_audioProcessor.apvts, lowCutFreqParamID.getParamID() };
    RotaryKnob m_lowCutQKnob{ "Low Cut Q", m_audioProcessor.apvts, lowCutQParamID.getParamID() };
    RotaryKnob m_highCutFreqKnob{ "High Cut Freq", m_audioProcessor.apvts, highCutFreqParamID.getParamID() };
    RotaryKnob m_highCutQKnob{ "High Cut Q", m_audioProcessor.apvts, highCutQParamID.getParamID() };

    RotaryKnob m_gainKnob{ "Gain", m_audioProcessor.apvts, gainParamID.getParamID() };
    RotaryKnob m_mixKnob{ "Mix", m_audioProcessor.apvts, mixParamID.getParamID() };

    RotaryKnob m_chorusIntensityKnob{ "Intensity", m_audioProcessor.apvts, chorusIntensityParamID.getParamID() };
	RotaryKnob m_chorusModRateKnob{ "Mod Rate", m_audioProcessor.apvts, chorusModRateParamID.getParamID() };
	RotaryKnob m_chorusModDepthKnob{ "Mod Depth", m_audioProcessor.apvts, chorusModDepthParamID.getParamID() };

    juce::GroupComponent m_delayGroup, m_filterGroup, m_outputGroup, m_chorusGroup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViiveAudioProcessorEditor)
};
