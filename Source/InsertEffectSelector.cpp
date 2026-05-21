#include "InsertEffectSelector.h"

InsertEffectSelector::InsertEffectSelector()
{
    m_currentType = InsertEffectType::Chorus;
    m_active = &m_chorus;
}
 
void InsertEffectSelector::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
    m_chorus.prepareToPlay(sampleRate, samplesPerBlock);
    m_lofi.prepareToPlay(sampleRate, samplesPerBlock);
}

void InsertEffectSelector::reset() noexcept
{
    m_chorus.reset();
    m_lofi.reset();
}

void InsertEffectSelector::processSample(const float& inL, const float& inR, float& outL, float& outR)
{
    if (m_active)
        m_active->processSample(inL, inR, outL, outR);
}

void InsertEffectSelector::update(const Parameters& params)
{
    if (m_active)
        m_active->update(params);
}

void InsertEffectSelector::select(InsertEffectType type)
{
    InsertEffect* options[] = { &m_chorus, &m_lofi };
    m_active = options[static_cast<int>(type)];
    m_currentType = type;
    m_active->reset();
}