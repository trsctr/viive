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
    : m_measurementL(measurementL), m_measurementR(measurementR)
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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("LevelMeter", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void LevelMeter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void LevelMeter::timerCallback()
{
    DBG("Left: " << m_measurementL.load() << " Right: " << m_measurementR.load());
}
