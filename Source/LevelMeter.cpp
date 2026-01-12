/*
  ==============================================================================

    LevelMeter.cpp
    Created: 12 Jan 2026 10:57:30am
    Author:  trise

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LevelMeter.h"

//==============================================================================
LevelMeter::LevelMeter(std::atomic<float>& measurementL, std::atomic<float>& measurementR)
    : m_measurementL(measurementL), m_measurementR(measurementR), m_dbLevelL(clampdB), m_dbLevelR(clampdB)
{
    setOpaque(true);
    startTimerHz(1);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    g.fillAll(juce::Colours::darkgrey);
    drawLevel(g, m_dbLevelL, 0, 7);
    drawLevel(g, m_dbLevelR, 9, 7);
    g.setFont(10.0f);
    for (float db = maxdB; db >= mindB; db -= stepdB) {
        int y = positionForLevel(db);

        g.setColour(juce::Colours::lightgrey);
        g.fillRect(0, y, 16, 1);

        g.setColour(juce::Colours::white);
        g.drawSingleLineText(juce::String(static_cast<int>(db)), bounds.getWidth(), y + 3, juce::Justification::right);
    }
}

void LevelMeter::resized()
{
    m_maxPos = 4.0f;
    m_minPos = float(getHeight()) - 4.0f;
}

void LevelMeter::timerCallback()
{
    m_dbLevelL = juce::Decibels::gainToDecibels(m_measurementL.load(), clampdB);
    m_dbLevelR = juce::Decibels::gainToDecibels(m_measurementR.load(), clampdB);
    repaint();
}

void LevelMeter::drawLevel(juce::Graphics& g, float level, int x, int width)
{
    int y = positionForLevel(level);
    if (level > 0.0f) {
        int y0 = positionForLevel(0.0f);
        g.setColour(juce::Colours::red);
        g.fillRect(x, y, width, y0 - y);
        g.setColour(juce::Colours::lightgreen);
        g.fillRect(x, y0, width, getHeight() - y0);
    }
    else if (y < getHeight()) {
        g.setColour(juce::Colours::lightgreen);
        g.fillRect(x, y, width, getHeight() - y);
    }
}
