#pragma once

enum Channel { Left = 0, Right = 1 };

// keep these syncronized with the string arrays in Parameters.cpp
// Parameters::delayModes
enum class DelayMode { Stereo, Cross, PingPongLR, PingPongRL};

// Parameters::lfoShapes
enum class LFOShape { Sine, Triangle, Square, SawUp, SawDown, SampleAndHold };

// Parameters::insertEffects
enum class InsertEffectType { Chorus, Lofi, RingMod };
