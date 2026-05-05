#pragma once
#include <JuceHeader.h>

class LFOIndicator : public juce::Component, private juce::Timer
{
public:
    LFOIndicator(std::atomic<float>& value) : m_value(value)
    {
        startTimerHz(60);
    }

    ~LFOIndicator() override { stopTimer(); }

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(1.0f);
        float v = m_value.load();
        juce::Colour fill;
        if (v >= 0.0f)
            fill = juce::Colours::black.interpolatedWith(juce::Colours::green, v);
        else
            fill = juce::Colours::black.interpolatedWith(juce::Colours::red, -v);

        g.setColour(juce::Colours::darkgrey.darker(0.5f));
        g.drawEllipse(bounds, 1.0f);
        g.setColour(fill);
        g.fillEllipse(bounds.reduced(1.0f));
    }

private:
    std::atomic<float>& m_value;
    void timerCallback() override { repaint(); }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOIndicator)
};
