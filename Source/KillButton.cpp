#include <JuceHeader.h>
#include "KillButton.h"

KillButton::KillButton(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID, int width, int height)
    : m_apvts(apvts), m_paramID(parameterID.getParamID())
{
    m_button.setButtonText(text);
    m_button.setClickingTogglesState(false);
    m_button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentWhite);
    m_button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::orange);
    m_button.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    m_button.setBounds(0, 0, width, height);
    m_button.addMouseListener(this, false);
    addAndMakeVisible(m_button);
    setSize(width, height);
}

KillButton::~KillButton()
{
    m_button.removeMouseListener(this);
}

void KillButton::resized()
{
    m_button.setTopLeftPosition(0, 0);
}

void KillButton::mouseDown(const juce::MouseEvent&)
{
    m_button.setToggleState(true, juce::dontSendNotification);
    if (auto* param = m_apvts.getParameter(m_paramID))
        param->setValueNotifyingHost(1.0f);
}

void KillButton::mouseUp(const juce::MouseEvent&)
{
    m_button.setToggleState(false, juce::dontSendNotification);
    if (auto* param = m_apvts.getParameter(m_paramID))
        param->setValueNotifyingHost(0.0f);
}
