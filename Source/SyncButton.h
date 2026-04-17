#pragma once

#include <JuceHeader.h>

class SyncButton  : public juce::Component
{
public:
    SyncButton(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID, int width = 40, int height = 20);
    ~SyncButton() override;

    void resized() override;

private:
    juce::TextButton m_button;

    juce::AudioProcessorValueTreeState::ButtonAttachment m_attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SyncButton)
};
