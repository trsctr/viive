# viive

A VST3 delay plugin built with JUCE. Work in progress.
Personal playground for learning audio DSP and experimenting with feedback-loop effects.
Currently developed and tested on Windows (JUCE AudioPluginHost, Ableton Live Suite 10).

![screenshot](./viive.png)

## Features

- Functional GUI with parameter controls and output level metering
- Saves and retrieves plugin state
- Adjustable delay time (5ms - 5000ms)
- Mix and feedback controls with feedback stabilization
- Sculpting Low/high-cut filters with adjustable resonance on delayed signal / feedback loop
- Cross-channel feedback for stereo effects
- Feedback loop protection: HPF, compressor, soft clipping

Despite the feedback protection, it can get quite loud and nasty.

Note: GUI is not final. Current knobs and their parameters are placeholders: Stereo, Effect Amt, FX Param 1, FX Param 2

## Potential future features

- Effects in feedback loop (phaser?, chorus?)
- LFO/modulation
- Tempo sync
- Preset manager

## Building

Requires JUCE and a C++20 compiler. Use the Projucer to generate build files for your platform.

## Based On

Based on concepts and example code from *The Complete Beginner’s Guide to Audio Plug-in Development* by Matthijs Hollemans.
