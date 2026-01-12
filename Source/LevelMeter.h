/*
  ==============================================================================

    LevelMeter.h
    Created: 12 Jan 2026 10:57:30am
    Author:  trise

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LevelMeter  : public juce::Component, private juce::Timer
{
public:
    LevelMeter(std::atomic<float>& measurementL, std::atomic<float>& measurementR);
    ~LevelMeter() override;

    static constexpr float  maxdB = 6.0f;
    static constexpr float  mindB = -60.0f;
    static constexpr float  stepdB = 6.0f;
    static constexpr float  clampdB = -120.0f;
    static constexpr float  clampLevel = 0.000001f;
    static constexpr int    refreshRate = 60;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    std::atomic<float>& m_measurementL;
    std::atomic<float>& m_measurementR;

    float m_maxPos = 0.0f;
    float m_minPos = 0.0f;
    float m_dbLevelL;
    float m_dbLevelR;

    float m_decay = 0.0f;
    float m_levelL = clampLevel;
    float m_levelR = clampLevel;

    void timerCallback() override;
    int positionForLevel(float dbLevel) const noexcept
    {
        return int(std::round(juce::jmap(dbLevel, maxdB, mindB, m_maxPos, m_minPos)));
    }
    void drawLevel(juce::Graphics& g, float level, int x, int width);
    void updateLevel(float newLevel, float& smoothedLevel, float& leveldB) const;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
