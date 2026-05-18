#include <JuceHeader.h>
#include "RotaryKnob.h"

juce::Font RotaryKnob::KnobLookAndFeel::getLabelFont(juce::Label&)
{
    return juce::Font(juce::FontOptions().withHeight(12.0f));
}

//==============================================================================
void RotaryKnob::KnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(4);
    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin(5.0f, radius * 0.3f);
    auto arcRadius = radius - lineW * 0.5f;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                arcRadius, arcRadius, 0.0f,
                                rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
    g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    if (slider.isEnabled())
    {
        juce::Path valueArc;
        valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                               arcRadius, arcRadius, 0.0f,
                               rotaryStartAngle, toAngle, true);
        g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
        g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    auto thumbWidth = lineW * 1.6f;  // default is lineW * 2.0f
    juce::Point<float> thumb(
        bounds.getCentreX() + arcRadius * std::cos(toAngle - juce::MathConstants<float>::halfPi),
        bounds.getCentreY() + arcRadius * std::sin(toAngle - juce::MathConstants<float>::halfPi));
    g.setColour(slider.findColour(juce::Slider::thumbColourId));
    g.fillEllipse(juce::Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumb));
}

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID, Size size)
	: m_attachment(apvts, parameterID.getParamID(), m_slider)
{
    const bool isSmall = size == Size::Small;
    const int w = isSmall ? 45 : 70;
    const int textBoxH = isSmall ? 12 : 16;
    const int gap = isSmall ? 0 : 2;
    const int labelH = isSmall ? 14 : 24;

    if (isSmall)
    {
        m_slider.setLookAndFeel(&m_laf);
        m_slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    }
    m_slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, w, textBoxH);
    m_slider.setBounds(0, 0, w, w + textBoxH + gap);
    addAndMakeVisible(m_slider);
    m_label.setText(text, juce::NotificationType::dontSendNotification);
    m_label.setFont(juce::Font(juce::FontOptions().withHeight(isSmall ? 13.0f : 14.0f)));
    m_label.setJustificationType(juce::Justification::horizontallyCentred);
    m_label.setBorderSize(juce::BorderSize<int>(0));
    m_label.attachToComponent(&m_slider, false);
    addAndMakeVisible(m_label);
    setSize(w, w + textBoxH + gap + labelH);
}

RotaryKnob::~RotaryKnob()
{
    m_slider.setLookAndFeel(nullptr);
}

void RotaryKnob::resized()
{
    m_slider.setTopLeftPosition(0, getHeight() - m_slider.getHeight());
}
