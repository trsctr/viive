#include "InsertEffectSelector.h"

InsertEffectSelector::InsertEffectSelector()
{
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
    switch(type) {
        case InsertEffectType::Chorus:
            m_active = &m_chorus;
            break;
        case InsertEffectType::Lofi:
            m_active = &m_lofi;
            break;
        default:
            jassertfalse; // this shouldn't happen unless the enum and string array get out of sync
            return;
    }
    m_active->reset();
}