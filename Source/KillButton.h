#pragma once

#include <JuceHeader.h>

class KillButton : public juce::Component
{
public:
    KillButton(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID, int width = 60, int height = 25);
    ~KillButton() override;

    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

private:
    juce::TextButton m_button;
    juce::AudioProcessorValueTreeState& m_apvts;
    juce::String m_paramID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KillButton)
};
