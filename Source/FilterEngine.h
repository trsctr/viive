#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "LFO.h"
#include "StereoFilter.h"
#include "Types.h"

using FilterType = juce::dsp::StateVariableTPTFilterType;

class FilterEngine
{
public:
    FilterEngine();
    ~FilterEngine() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
    void reset() noexcept;

    void update(const Parameters& params);
    void processSample(const float& inL, const float& inR, float& outL, float& outR);

private:
    StereoFilter m_lowCut{ FilterType::highpass };
    StereoFilter m_highCut{ FilterType::lowpass };

    LFO m_lowCutLfos[2]  = { {1.0f, 0.0f, LFOShape::Sine}, {1.0f, 0.0f, LFOShape::Sine} };
    LFO m_highCutLfos[2] = { {1.0f, 0.0f, LFOShape::Sine}, {1.0f, 0.0f, LFOShape::Sine} };

    float m_lowCutBase        = -1.0f;
    float m_lowCutQ           = -1.0f;
    float m_lowCutModDepth    = 0.0f;
    float m_lowCutLfoRate     = 0.0f;
    float m_lowCutPhaseOffset = 0.0f;

    float m_highCutBase        = -1.0f;
    float m_highCutQ           = -1.0f;
    float m_highCutModDepth    = 0.0f;
    float m_highCutLfoRate     = 0.0f;
    float m_highCutPhaseOffset = 0.0f;

    void setLowCutFreq(float freq);
    void setLowCutQ(float q);
    void setLowCutModDepth(float depthHz);
    void setLowCutLfoRate(float rate);
    void setLowCutPhaseOffset(float phase);

    void setHighCutFreq(float freq);
    void setHighCutQ(float q);
    void setHighCutModDepth(float depthHz);
    void setHighCutLfoRate(float rate);
    void setHighCutPhaseOffset(float phase);

    float modulatedCutoff(float base, float lfoValue, float modDepth);
    void setFilterFreq(float freq, float& current);
    void setFilterQ(float q, float& current, StereoFilter& filter);
    void setFilterModDepth(float depthHz, float& current);
    void setFilterLfoRate(float rate, float& current, LFO* lfos);
    void setFilterPhaseOffset(float phase, float& current, LFO* lfos);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEngine)
};
