#include <JuceHeader.h>
#include "SyncButton.h"

//==============================================================================
SyncButton::SyncButton(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID, int width, int height)
    : m_attachment(apvts, parameterID.getParamID(), m_button)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    m_button.setButtonText(text);
    m_button.setClickingTogglesState(true);
    m_button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentWhite);
    m_button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::orange);
    m_button.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    m_button.setBounds(0, 0, width, height);
    addAndMakeVisible(m_button);
    setSize(width, height);
}

SyncButton::~SyncButton()
{
}

void SyncButton::resized()
{
    m_button.setTopLeftPosition(0, 0);
}
