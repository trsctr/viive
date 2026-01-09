/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 9 Jan 2026 1:29:10pm
    Author:  trise

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID)
	: m_attachment(apvts, parameterID.getParamID(), m_slider)
{
    m_slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    m_slider.setBounds(0, 0, 70, 86);
    addAndMakeVisible(m_slider);
    m_label.setText(text, juce::NotificationType::dontSendNotification);
    m_label.setJustificationType(juce::Justification::horizontallyCentred);
    m_label.setBorderSize(juce::BorderSize<int>(0));
    m_label.attachToComponent(&m_slider, false);
    addAndMakeVisible(m_label);
    setSize(70, 110);
}

RotaryKnob::~RotaryKnob()
{
}

void RotaryKnob::resized()
{
    m_slider.setTopLeftPosition(0, 24);
}
