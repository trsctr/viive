#pragma once
#include <JuceHeader.h>
#include "LFOScopeBuffer.h"

class LFOScope : public juce::Component, private juce::Timer
{
public:
    LFOScope(LFOScopeBuffer& buffer);
    ~LFOScope() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    LFOScopeBuffer& m_buffer;

    static constexpr int refreshRate = 30;

    void timerCallback() override { repaint(); }
    void drawChannel(juce::Graphics& g, int channel, juce::Colour colour, int writeIdx, int numSamples);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOScope)
};
