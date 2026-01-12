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

### Requirements

* C++20 compiler (Visual Studio 2022 on Windows, Xcode on macOS)
* JUCE **8.0.11**
* Projucer

### 1) Get JUCE 8.0.11

Download JUCE **8.0.11** from the [official JUCE GitHub releases](https://github.com/juce-framework/JUCE/releases/tag/8.0.11)

You should end up with a folder that contains:

* `JUCE/modules/`
* `Projucer` (the Projucer app)

### 2) Set the JUCE module path in Projucer

Open **Projucer** → FILE → **Global paths...**:

Make sure the JUCE Modules path points to:
* `<path-to-JUCE>/modules`

### 3) Generate IDE project files

* Open `viive.jucer` in Projucer
* Click **Save Project**

This generates platform-specific projects under `Builds/`

### 4) Build

Select your exporter from the menu in projucer and click the logo to open the project in Xcode/Visual Studio
Once the project opens, build the `Release` configuration.

### Output

* VST3 will be produced under the corresponding `Builds/...` directory (search for `*.vst3` if you’re unsure).

## Based On

Based on concepts and example code from *The Complete Beginner’s Guide to Audio Plug-in Development* by Matthijs Hollemans.
