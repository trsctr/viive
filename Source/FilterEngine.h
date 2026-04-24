#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "LFO.h"
#include "StereoFilter.h"
#include "Types.h"
#include "DSP.h"
#if JUCE_DEBUG
#include "LFOScopeBuffer.h"
#endif

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

    void updateLFOs(const double ppqPosition) noexcept;

#if JUCE_DEBUG
    LFOScopeBuffer m_scopeBuffer;
#endif

private:
    StereoFilter m_lowCut{ FilterType::highpass };
    StereoFilter m_highCut{ FilterType::lowpass };
    juce::Random m_random;

    LFO m_lowCutLfos[2];
    LFO m_highCutLfos[2];

    float m_lowCutBase        = -1.0f;
    float m_lowCutQ           = -1.0f;
    float m_lowCutModDepth    = 0.0f;
    float m_lowCutModRate     = 0.0f;
    float m_lowCutModPhase    = 0.0f;
    LFOShape m_lowCutModShape = LFOShape::Sine;
    bool m_lowCutModTempoSync = false;
    int m_lowCutModNote = 0;

    float m_highCutBase        = -1.0f;
    float m_highCutQ           = -1.0f;
    float m_highCutModDepth    = 0.0f;
    float m_highCutModRate     = 0.0f;
    float m_highCutModPhase    = 0.0f;
    LFOShape m_highCutModShape = LFOShape::Sine;
    bool m_highCutModTempoSync = false;
    int m_highCutModNote = 0;

    bool m_highCutModInvert = false;

    float m_lowCutSmoothed[2]  = { Parameters::minFilterFreq, Parameters::minFilterFreq };
    float m_highCutSmoothed[2] = { Parameters::maxFilterFreq, Parameters::maxFilterFreq };
    float m_cutoffSmoothCoeff  = 0.0f;

#if JUCE_DEBUG
    static constexpr int scopeDownsample = 64;
    int m_scopeCounter = 0;
#endif

    void setLowCutFilter(const Parameters& params);
    void setHighCutFilter(const Parameters& params);

    float modulatedCutoff(float base, float lfoValue, float modDepth);
    void setFilterFreq(float freq, float& current);
    void setFilterQ(float q, float& current, StereoFilter& filter);
    void setFilterModDepth(float depth, float& current);
    void setFilterModRate(float rate, float& current, LFO* lfos);
    void setFilterModPhase(float phase, float& current, LFO* lfos);
    void setFilterModShape(int shapeIndex, LFOShape& current, LFO* lfos);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEngine)
};
