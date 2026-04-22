#include "LFOScope.h"

LFOScope::LFOScope(LFOScopeBuffer& buffer) : m_buffer(buffer)
{
    startTimerHz(refreshRate);
}

LFOScope::~LFOScope()
{
    stopTimer();
}

void LFOScope::resized() {}

void LFOScope::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    int w = bounds.getWidth();
    int h = bounds.getHeight();

    g.fillAll(juce::Colour(0xff1a1a1a));

    g.setColour(juce::Colour(0xff333333));
    g.drawHorizontalLine(h / 2, 0.0f, (float)w);
    g.drawHorizontalLine(h / 4, 0.0f, (float)w);
    g.drawHorizontalLine(3 * h / 4, 0.0f, (float)w);
    g.drawRect(bounds);

    int numSamples = std::min(w, LFOScopeBuffer::size);
    int writeIdx = m_buffer.writeIndex.load(std::memory_order_acquire);

    const juce::Colour colours[LFOScopeBuffer::numChannels] = {
        juce::Colour(0xff4fc3f7),  // lowcut L  — light blue
        juce::Colour(0xff0288d1),  // lowcut R  — dark blue
        juce::Colour(0xffffb74d),  // highcut L — amber
        juce::Colour(0xfff57c00),  // highcut R — dark amber
    };

    const char* labels[LFOScopeBuffer::numChannels] = {
        "LC L", "LC R", "HC L", "HC R"
    };

    for (int ch = 0; ch < LFOScopeBuffer::numChannels; ++ch)
        drawChannel(g, ch, colours[ch], writeIdx, numSamples);

    g.setFont(11.0f);
    for (int ch = 0; ch < LFOScopeBuffer::numChannels; ++ch) {
        g.setColour(colours[ch]);
        g.drawText(labels[ch], 4 + ch * 48, 4, 44, 14, juce::Justification::left);
    }
}

void LFOScope::drawChannel(juce::Graphics& g, int channel, juce::Colour colour, int writeIdx, int numSamples)
{
    int h = getHeight();
    juce::Path path;

    for (int x = 0; x < numSamples; ++x) {
        int bufIdx = (writeIdx - numSamples + x + LFOScopeBuffer::size) & (LFOScopeBuffer::size - 1);
        float value = m_buffer.data[channel][bufIdx];
        float y = juce::jmap(value, -1.0f, 1.0f, (float)(h - 1), 0.0f);

        if (x == 0)
            path.startNewSubPath(0.0f, y);
        else
            path.lineTo((float)x, y);
    }

    g.setColour(colour);
    g.strokePath(path, juce::PathStrokeType(1.5f));
}
