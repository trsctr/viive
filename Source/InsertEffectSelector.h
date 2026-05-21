#pragma once
#include "InsertEffect.h"
#include "ChorusEngine.h"
#include "LofiEngine.h"
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

private:
    ChorusEngine m_chorus;
    LofiEngine m_lofi;
    InsertEffect* m_active = nullptr;
    InsertEffectType m_currentType;


};