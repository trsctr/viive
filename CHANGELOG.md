# Changelog

All notable changes to this project will be documented in this file.

## \[Unreleased]

### Planned

* Tempo sync for the delay time
* Cross feedback control parameter
* Additional effects in feedback loop (phaser, bitcrusher, etc.)
* Preset manager

## \[0.0.5] - 2026-02-26

### Added

* Delay engine with adjustable delay time (5ms - 5000ms)
* Mix and feedback controls with feedback stabilization
* Sculpting low/high-cut filters with adjustable resonance on delayed signal and feedback loop
* Cross-channel feedback for stereo effects
* Dual-voice chorus effect in feedback loop with exponential intensity mapping
* Chorus parameter controls: intensity, modulation rate, modulation depth
* Feedback loop protection: high-pass filter, compressor, soft clipping
* LFO class with sine and triangle waveforms for effect modulation
* Output level metering with smooth drawing (60Hz refresh rate)
* Plugin state saving and retrieval
* Placeholder GUI with parameters and knobs

### Technical

* DSP utilities: one-pole lowpass filter, coefficient calculation, sample conversion
* Chorus makeup gain normalization to prevent clipping when wet signal is added
* One-pole lowpass filtering used for parameter smoothing, DC blocking in chorus, and meter drawing smoothness

### Known Limitations

* GUI is placeholder and subject to major redesign
* No tempo sync yet
* Limited effect selection (only chorus in feedback loop)
