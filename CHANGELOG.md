# Changelog

All notable changes to this project will be documented in this file.

## \[Unreleased]

### Planned

* Additional effects in feedback loop (phaser, bitcrusher, etc.)
* Preset manager
* Ducking
* LFO/Modulation for general parameters (delay time, filter cutoff)

## \[0.1.0] - 2026-04-17

### Added

* Tempo sync
* Separate L/R delay time parameters
* Offset parameter for stereo offset between delay channels
* Stereo width control with mid/side processing
* Selectable delay modes: stereo, cross feedback, ping pong

### Changed

* Refactored delay line into StereoDelay primitive (two independent mono delay lines)
* Cross feedback moved from hardcoded behavior to selectable delay mode
* Removed negative feedback and set feedback range to 0-140%

### Known limitations

* GUI still placeholder and will require a redesign
* Limited effect selection for feedback loop (only chorus)
* Feedback distorts or gets strange artifacts on some filter settings or certain kind of input
* Feedback gets strange noise on high (over 100%) feedback settings. Not necessarily a bad thing.
* Delay time changes might introduce some strange artifacts
* Left/Right delay parameters can't be linked as it caused some thread safety issues, possibly implementing a switch between single and dual modes

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
