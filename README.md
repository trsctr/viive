# viive

A VST3 delay plugin built with JUCE. Work in progress.
Personal playground for experimenting with different effects in feedback loop and just learning audio programming.
Currently developed and tested on Windows with JUCE Audioplugin host and Ableton Live Suite 10.

## Features

- Saves and retrieves plugin state
- Adjustable delay time (5ms - 5000ms)
- Mix and feedback controls with feedback stabilization
- Sculpting Low/high-cut filters with adjustable resonance on delayed signal / feedback loop
- Cross-channel feedback for stereo effects
- Feedback loop protection: HPF, compressor, soft clipping

Despite of the feedback protection, it can get quite loud and nasty.

## Possibly planned features

- GUI
- Effects in feedback loop (phaser?, chorus?)
- LFO/modulation
- Tempo sync
- Preset manager

## Building

Requires JUCE and a C++20 compiler. Use the Projucer to generate build files for your platform.

## Based On

Built on concepts and patterns from "The Complete Beginner's Guide to Audio Plug-in Development" by Matthijs Hollemans.
