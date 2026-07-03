#pragma once
#include "InsertEffect.h"
#include "ChorusEngine.h"
#include "LofiEngine.h"
#include "RingModEngine.h"
#include "Types.h"

class InsertEffectSelector
{
public:
    InsertEffectSelector();
    ~InsertEffectSelector() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
    void reset() noexcept;
    void processSample(const float& inL, const float& inR, float& outL, float& outR);
    void update(const Parameters& params);
    void select(InsertEffectType type);
    void kill();

private:
    ChorusEngine m_chorus;
    LofiEngine m_lofi;
    RingModEngine m_ringMod;

    InsertEffect* m_active = nullptr;

    void setFxEnabled(bool enabled);

    bool m_fxEnabled = true;
};