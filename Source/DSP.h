#pragma once
#include <cmath>
#include <JuceHeader.h>

inline float onePoleLowpassCoeff(float timeConstantMs, float sampleRate) {
	float tau = timeConstantMs / 1000.0f;
	jassert(tau > 0.0f); // tau can't be zero
	jassert(sampleRate > 0.0f); // sample rate can't be zero
	return 1.0f - std::exp(-1.0f / tau * sampleRate);
}

inline float onePoleLowpass(float input, float& state, float coeff) {
	state += coeff * (input - state);
	return state;
}