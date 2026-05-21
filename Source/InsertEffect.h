#pragma once

#include "Parameters.h"

class InsertEffect
{
public:
    InsertEffect() = default;
    virtual ~InsertEffect() = default;

    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept = 0;
    virtual void reset() noexcept = 0;
    virtual void processSample(const float& inL, const float& inR, float& outL, float& outR) = 0;
    virtual void update(const Parameters& params) = 0;
};