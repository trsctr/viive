/*
  ==============================================================================

    Button.h
    Created: 15 Apr 2026 11:58:15am
    Author:  trise

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Button  : public juce::Component
{
public:
    Button(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID, int width = 40, int height = 20);
    ~Button() override;

    void resized() override;

private:
    juce::TextButton m_button;

    juce::AudioProcessorValueTreeState::ButtonAttachment m_attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Button)
};
