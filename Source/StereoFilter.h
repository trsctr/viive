#pragma once
#include <JuceHeader.h>
#include "Types.h"

using Filter = juce::dsp::StateVariableTPTFilter<float>;
using FilterType = juce::dsp::StateVariableTPTFilterType;

class StereoFilter
{
public:
	StereoFilter(FilterType type = FilterType::lowpass);
	~StereoFilter() = default;

	void prepareToPlay(double sampleRate, int samplesPerBlock) noexcept;
	void reset() noexcept;

	void processSample(const float& inL, const float& inR, float& outL, float& outR);
	
	void setCutoff(float freq);
	void setCutoff(float freq, Channel channel);
	void setQ(float q);

private:
	Filter m_filters[2];

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoFilter)
};
