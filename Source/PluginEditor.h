/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "Rotaryknob.h"

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
    ViiveAudioProcessor& audioProcessor;

	RotaryKnob m_gainKnob{ "Gain", audioProcessor.apvts, gainParamID.getParamID() };
    
    juce::GroupComponent m_delayGroup, m_feedbackGroup, m_filterGroup, m_outputGroup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViiveAudioProcessorEditor)
};
