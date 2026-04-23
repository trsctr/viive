#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RotaryKnob  : public juce::Component
{
public:
    enum class Size { Normal, Small };

    RotaryKnob(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID, Size size = Size::Normal);
    ~RotaryKnob() override;

    void resized() override;
	juce::Slider& getSlider() { return m_slider; }
	juce::Label& getLabel() { return m_label; }

private:
    struct KnobLookAndFeel : public juce::LookAndFeel_V4
    {
        void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                              float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                              juce::Slider&) override;
        juce::Font getLabelFont(juce::Label&) override;
    };

    KnobLookAndFeel m_laf;
    juce::Slider m_slider;
    juce::Label m_label;

    juce::AudioProcessorValueTreeState::SliderAttachment m_attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
