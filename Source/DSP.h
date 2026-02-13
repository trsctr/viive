#pragma once
#include <cmath>
#include <JuceHeader.h>

inline float onePoleLowpassCoeff(float timeConstantMs, float sampleRate) {
	float tau = timeConstantMs / 1000.0f;
	jassert(tau > 0.0f); // tau can't be zero
	jassert(sampleRate > 0.0f); // sample rate can't be zero
	float x = std::exp(-1.0f / (tau * sampleRate));
	return 1.0f - x;
}

inline float onePoleLowpass(float input, float& state, float coeff) {
	state += (input - state) * coeff;
	return state;
}

inline float msToSamples(float ms, float sampleRate) {
	return ms * sampleRate * 0.001f;
}