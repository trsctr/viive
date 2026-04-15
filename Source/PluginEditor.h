#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "LevelMeter.h"

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
    virtual void parameterChanged(const juce::String& paramID, float newValue) override;
    void linkParameters(const juce::String& idA, const juce::String& idB);

private:
    ViiveAudioProcessor& m_audioProcessor;
    LevelMeter m_meter;

    RotaryKnob m_delayTimeLKnob{ "Time L", m_audioProcessor.apvts, delayTimeLParamID.getParamID() };
    RotaryKnob m_delayTimeRKnob{ "Time R", m_audioProcessor.apvts, delayTimeRParamID.getParamID() };
    RotaryKnob m_feedbackKnob{ "Feedback", m_audioProcessor.apvts, feedbackParamID.getParamID() };
    RotaryKnob m_stereoKnob{ "Stereo", m_audioProcessor.apvts, stereoParamID.getParamID() };
    RotaryKnob m_spreadKnob{ "Spread", m_audioProcessor.apvts, spreadParamID.getParamID() };

    RotaryKnob m_lowCutFreqKnob{ "Low Cut Freq", m_audioProcessor.apvts, lowCutFreqParamID.getParamID() };
    RotaryKnob m_lowCutQKnob{ "Low Cut Q", m_audioProcessor.apvts, lowCutQParamID.getParamID() };
    RotaryKnob m_highCutFreqKnob{ "High Cut Freq", m_audioProcessor.apvts, highCutFreqParamID.getParamID() };
    RotaryKnob m_highCutQKnob{ "High Cut Q", m_audioProcessor.apvts, highCutQParamID.getParamID() };

    RotaryKnob m_gainKnob{ "Gain", m_audioProcessor.apvts, gainParamID.getParamID() };
    RotaryKnob m_mixKnob{ "Mix", m_audioProcessor.apvts, mixParamID.getParamID() };

    RotaryKnob m_chorusIntensityKnob{ "Intensity", m_audioProcessor.apvts, chorusIntensityParamID.getParamID() };
	RotaryKnob m_chorusModRateKnob{ "Mod Rate", m_audioProcessor.apvts, chorusModRateParamID.getParamID() };
	RotaryKnob m_chorusModDepthKnob{ "Mod Depth", m_audioProcessor.apvts, chorusModDepthParamID.getParamID() };

    juce::TextButton m_delayLinkButton;

    juce::HashMap<juce::String, juce::String> m_linkedParams;

    juce::AudioProcessorValueTreeState::ButtonAttachment linkAttachment{
        m_audioProcessor.apvts, delayLinkParamID.getParamID(), m_delayLinkButton
    };

    juce::GroupComponent m_delayGroup, m_filterGroup, m_outputGroup, m_chorusGroup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViiveAudioProcessorEditor)
};
