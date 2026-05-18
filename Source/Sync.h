#pragma once
#include <JuceHeader.h>

inline static std::array<double, 16> delayNoteLengthMultipliers =
{
	0.125,          // 0 = 1/32
	0.5 / 3.0,      // 1 = 1/16 triplet
	0.1875,         // 2 = 1/32 dotted
	0.25,           // 3 = 1/16
	1.0 / 3.0,      // 4 = 1/8 triplet
	0.375,          // 5 = 1/16 dotted
	0.5,            // 6 = 1/8
	2.0 / 3.0,      // 7 = 1/4 triplet
	0.75,           // 8 = 1/8 dotted
	1.0,            // 9 = 1/4
	4.0 / 3.0,      // 10 = 1/2 triplet
	1.5,            // 11 = 1/4 dotted
	2.0,            // 12 = 1/2
	8.0 / 3.0,      // 13 = 1/1 triplet
	3.0,            // 14 = 1/2 dotted
	4.0,            // 15 = 1/1
};

inline static std::array<double, 15> modNoteLengthMultipliers =
{
	0.5,            // 0  = 1/8
	0.75,           // 1  = 1/8 dotted
	1.0,            // 2  = 1/4
	1.5,            // 3  = 1/4 dotted
	2.0,            // 4  = 1/2
	3.0,            // 5  = 1/2 dotted
	4.0,            // 6  = 1/1
	6.0,            // 7  = 1/1 dotted
	8.0,            // 8  = 2/1
	12.0,           // 9  = 3/1
	16.0,           // 10 = 4/1
	20.0,           // 11 = 5/1
	24.0,           // 12 = 6/1
	28.0,           // 13 = 7/1
	32.0,           // 14 = 8/1
};

inline float getSyncedPhase(int index, double ppqPosition)
{
	double cycleLengthInBeats = modNoteLengthMultipliers[size_t(index)];
	return static_cast<float>(std::fmod(ppqPosition, cycleLengthInBeats) / cycleLengthInBeats);
}
