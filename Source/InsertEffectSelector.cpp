#include "InsertEffectSelector.h"

InsertEffectSelector::InsertEffectSelector()
{
    m_active = &m_chorus;
}
 
void InsertEffectSelector::prepareToPlay(double sampleRate, int samplesPerBlock) noexcept
{
    m_chorus.prepareToPlay(sampleRate, samplesPerBlock);
    m_lofi.prepareToPlay(sampleRate, samplesPerBlock);
    m_ringMod.prepareToPlay(sampleRate, samplesPerBlock);
}

void InsertEffectSelector::reset() noexcept
{
    m_chorus.reset();
    m_lofi.reset();
    m_ringMod.reset();
}

void InsertEffectSelector::processSample(const float& inL, const float& inR, float& outL, float& outR)
{

    if (m_active)
        m_active->processSample(inL, inR, outL, outR);
    if (!m_fxEnabled)
    {
        outL = inL;
        outR = inR;
    }
}

void InsertEffectSelector::update(const Parameters& params)
{
    if (m_active)
        m_active->update(params);
    setFxEnabled(params.fxEnabled());
}

void InsertEffectSelector::setFxEnabled(bool enabled)
{
    m_fxEnabled = enabled;
}

void InsertEffectSelector::kill()
{
    if (m_active)
        m_active->kill();
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
        case InsertEffectType::RingMod:
            m_active = &m_ringMod;
            break;
        default:
            jassertfalse; // this shouldn't happen unless the enum and string array get out of sync
            return;
    }
    m_active->reset();
}