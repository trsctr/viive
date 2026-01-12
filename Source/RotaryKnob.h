#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RotaryKnob  : public juce::Component
{
public:
    RotaryKnob(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID);
    ~RotaryKnob() override;

    void resized() override;
	juce::Slider& getSlider() { return m_slider; }
	juce::Label& getLabel() { return m_label; }

private:
    juce::Slider m_slider;
    juce::Label m_label;

    juce::AudioProcessorValueTreeState::SliderAttachment m_attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
