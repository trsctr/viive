#pragma once

#include <JuceHeader.h>

class Selector : public juce::Component
{
public:
    Selector(const juce::String& labelText,
             juce::AudioProcessorValueTreeState& apvts,
             const juce::ParameterID& parameterID,
             const juce::StringArray& items,
             int width = 60,
             float fontSize = 0.0f,
             int leftLabelWidth = 0,
             int height = 0);
    ~Selector() override;

    void resized() override;

private:
    static constexpr int s_labelH = 14;
    static constexpr int s_comboH = 20;

    struct SelectorLookAndFeel : public juce::LookAndFeel_V4
    {
        float fontSize = 0.0f;
        juce::Font getComboBoxFont(juce::ComboBox&) override
        {
            return juce::Font(juce::FontOptions().withHeight(fontSize));
        }
    };

    SelectorLookAndFeel m_laf;
    juce::Label m_label;
    juce::ComboBox m_comboBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> m_attachment;
    bool m_hasLabel;
    int m_leftLabelWidth = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Selector)
};
