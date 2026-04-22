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
}

float FilterEngine::modulatedCutoff(float base, float lfoValue, float modDepth)
{
    return juce::jlimit(Parameters::minFilterFreq, Parameters::maxFilterFreq, base + lfoValue * modDepth);
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

void FilterEngine::setFilterModDepth(float depthHz, float& current)
{
    if (!juce::approximatelyEqual(depthHz, current))
        current = depthHz;
}

void FilterEngine::setFilterLfoRate(float rate, float& current, LFO* lfos)
{
    if (!juce::approximatelyEqual(rate, current)) {
        current = rate;
        for (int i = 0; i < 2; i++)
            lfos[i].setFrequency(current);
    }
}

void FilterEngine::setFilterPhaseOffset(float phase, float& current, LFO* lfos)
{
    if (!juce::approximatelyEqual(phase, current)) {
        current = phase;
        float normalized = std::fmod(std::fmod(phase, 360.0f) + 360.0f, 360.0f);
        lfos[Channel::Right].setOffset(normalized);
    }
}

void FilterEngine::setLowCutFreq(float freq)
{
    setFilterFreq(freq, m_lowCutBase);
}

void FilterEngine::setLowCutQ(float q)
{
    setFilterQ(q, m_lowCutQ, m_lowCut);
}

void FilterEngine::setLowCutModDepth(float depth)
{
    setFilterModDepth(depth, m_lowCutModDepth);
}

void FilterEngine::setLowCutLfoRate(float rate)
{
    setFilterLfoRate(rate, m_lowCutLfoRate, m_lowCutLfos);
}

void FilterEngine::setLowCutPhaseOffset(float phase)
{
    setFilterPhaseOffset(phase, m_lowCutPhaseOffset, m_lowCutLfos);
}

void FilterEngine::setHighCutFreq(float freq)
{
    setFilterFreq(freq, m_highCutBase);
}

void FilterEngine::setHighCutQ(float q)
{
    setFilterQ(q, m_highCutQ, m_highCut);
}

void FilterEngine::setHighCutModDepth(float depth)
{
    setFilterModDepth(depth, m_highCutModDepth);
}

void FilterEngine::setHighCutLfoRate(float rate)
{
    setFilterLfoRate(rate, m_highCutLfoRate, m_highCutLfos);
}

void FilterEngine::setHighCutPhaseOffset(float phase)
{
    setFilterPhaseOffset(phase, m_highCutPhaseOffset, m_highCutLfos);
}

void FilterEngine::update(const Parameters& params)
{
    setLowCutFreq(params.lowCutFreq());
    setLowCutQ(params.lowCutQ());
    setHighCutFreq(params.highCutFreq());
    setHighCutQ(params.highCutQ());
}

void FilterEngine::processSample(const float& inL, const float& inR, float& outL, float& outR)
{
    m_lowCut.setCutoff(m_lowCutBase   + m_lowCutLfos[Channel::Left].process()   * m_lowCutModDepth,  Channel::Left);
    m_lowCut.setCutoff(m_lowCutBase   + m_lowCutLfos[Channel::Right].process()  * m_lowCutModDepth,  Channel::Right);
    m_highCut.setCutoff(m_highCutBase + m_highCutLfos[Channel::Left].process()  * m_highCutModDepth, Channel::Left);
    m_highCut.setCutoff(m_highCutBase + m_highCutLfos[Channel::Right].process() * m_highCutModDepth, Channel::Right);

    float lowL, lowR;
    m_lowCut.processSample(inL, inR, lowL, lowR);
    m_highCut.processSample(lowL, lowR, outL, outR);
}
