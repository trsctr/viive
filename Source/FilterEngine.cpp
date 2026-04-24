#include "FilterEngine.h"

FilterEngine::FilterEngine()
{
}

void FilterEngine::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
    m_lowCut.prepareToPlay(sampleRate, samplesPerBlock);
    m_highCut.prepareToPlay(sampleRate, samplesPerBlock);
    for (auto& lfo : m_lowCutLfos)
        lfo.prepareToPlay(static_cast<float>(sampleRate));
    for (auto& lfo : m_highCutLfos)
        lfo.prepareToPlay(static_cast<float>(sampleRate));
    m_cutoffSmoothCoeff = onePoleLowpassCoeff(5.0f, static_cast<float>(sampleRate));
}

void FilterEngine::reset() noexcept
{
    m_lowCut.reset();
    m_highCut.reset();
    for (auto& lfo : m_lowCutLfos)
        lfo.reset();
    for (auto& lfo : m_highCutLfos)
        lfo.reset();
    m_lowCutBase  = -1.0f;
    m_lowCutQ     = -1.0f;
    m_highCutBase = -1.0f;
    m_highCutQ    = -1.0f;
    m_lowCutSmoothed[0]  = m_lowCutSmoothed[1]  = Parameters::minFilterFreq;
    m_highCutSmoothed[0] = m_highCutSmoothed[1] = Parameters::maxFilterFreq;
}

float FilterEngine::modulatedCutoff(float base, float lfoValue, float modDepth)
{
    return juce::jlimit(Parameters::minFilterFreq, Parameters::maxFilterFreq, base * std::pow(2.0f, lfoValue * modDepth));
}

void FilterEngine::setFilterFreq(float freq, float& current)
{
    if (!juce::approximatelyEqual(freq, current))
        current = freq;
}

void FilterEngine::setFilterQ(float q, float& current, StereoFilter& filter)
{
    float newQ = std::min(q, Parameters::maxFilterQ);
    if (!juce::approximatelyEqual(newQ, current)) {
        current = newQ;
        filter.setQ(current);
    }
}

void FilterEngine::setFilterModDepth(float depth, float& current)
{
    if (!juce::approximatelyEqual(depth, current))
        current = depth;
}

void FilterEngine::setFilterModRate(float rate, float& current, LFO* lfos)
{
    if (!juce::approximatelyEqual(rate, current)) {
        current = rate;
        for (int i = 0; i < 2; i++)
            lfos[i].setFrequency(current);
    }
}

void FilterEngine::setFilterModPhase(float phase, float& current, LFO* lfos)
{
    if (!juce::approximatelyEqual(phase, current)) {
        current = phase;
        float normalized = std::fmod(std::fmod(phase, 360.0f) + 360.0f, 360.0f) / 360.0f;
        lfos[Channel::Right].setOffset(normalized);
    }
}

void FilterEngine::setLowCutFilter(const Parameters& params)
{
    setFilterFreq(params.lowCutFreq(), m_lowCutBase);
    setFilterQ(params.lowCutQ(), m_lowCutQ, m_lowCut);
    setFilterModRate(params.lowCutModRate(), m_lowCutModRate, m_lowCutLfos);
    setFilterModDepth(params.lowCutModDepth(), m_lowCutModDepth);
    setFilterModPhase(params.lowCutModPhase(), m_lowCutModPhase, m_lowCutLfos );
}

void FilterEngine::setHighCutFilter(const Parameters& params)
{
    setFilterFreq(params.highCutFreq(), m_highCutBase);
    setFilterQ(params.highCutQ(), m_highCutQ, m_highCut);
    setFilterModRate(params.highCutModRate(), m_highCutModRate, m_highCutLfos);
    setFilterModDepth(params.highCutModDepth(), m_highCutModDepth);
    setFilterModPhase(params.highCutModPhase(), m_highCutModPhase, m_highCutLfos);
}


void FilterEngine::update(const Parameters& params)
{
    setLowCutFilter(params);
    setHighCutFilter(params);
    m_highCutModInvert = params.highCutModInvert();
}

void FilterEngine::processSample(const float& inL, const float& inR, float& outL, float& outR)
{
    float lcL = m_lowCutLfos[Channel::Left].process();
    float lcR = m_lowCutLfos[Channel::Right].process();
    float hcL = m_highCutLfos[Channel::Left].process();
    float hcR = m_highCutLfos[Channel::Right].process();

    hcL = m_highCutModInvert ? -hcL : hcL;
    hcR = m_highCutModInvert ? -hcR : hcR;

#if JUCE_DEBUG
    if (++m_scopeCounter >= scopeDownsample) {
        m_scopeCounter = 0;
        m_scopeBuffer.push(lcL, lcR, hcL, hcR);
    }
#endif

    m_lowCut.setCutoff(onePoleLowpass(modulatedCutoff(m_lowCutBase,  lcL, m_lowCutModDepth),  m_lowCutSmoothed[Channel::Left],   m_cutoffSmoothCoeff), Channel::Left);
    m_lowCut.setCutoff(onePoleLowpass(modulatedCutoff(m_lowCutBase,  lcR, m_lowCutModDepth),  m_lowCutSmoothed[Channel::Right],  m_cutoffSmoothCoeff), Channel::Right);
    m_highCut.setCutoff(onePoleLowpass(modulatedCutoff(m_highCutBase, hcL, m_highCutModDepth), m_highCutSmoothed[Channel::Left],  m_cutoffSmoothCoeff), Channel::Left);
    m_highCut.setCutoff(onePoleLowpass(modulatedCutoff(m_highCutBase, hcR, m_highCutModDepth), m_highCutSmoothed[Channel::Right], m_cutoffSmoothCoeff), Channel::Right);

    float lowL, lowR;
    m_lowCut.processSample(inL, inR, lowL, lowR);
    m_highCut.processSample(lowL, lowR, outL, outR);
}
