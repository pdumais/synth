# Modular Synthesizer

# Design
- 5V system, but requires +12,-12 rails for modules using opamps
- uses computer PSU
- 2 signal paths:
    1) The audio signal. -5 - +5V. 
    2) Control Voltages. They work on unipolar 5V.

# Current modules
- [dual VCO and quad LFO. Square only](modules/osc)
- [LFO shaper: to convert square to trianble/sine or anything in between](modules/waveshaper)
- [Sequencer](modules/sequencer)
- [Clock](modules/clock)
- [Clock Divider](modules/divider)
- [Voltage Controlled Resistors](modules/digipot)
- [Midi2CV](modules/midi2cv)
- [Noise Generator](modules/noise)
- [4 Channel VCA](modules/vca)
- [4 stage VCF](modules/vcf)

# Known issues
- Power filtering
    The oscilloscope shows that the -5 and +5 rails oscillate at an amplitude of 0.4v. The wave has a period of 500ns.
    That's 2mhz. 
    For the CV path, this gets filtered out by the sampling rate of the ADC, so no need to do anything about it. When CV
    is used with analog, it wouldn't be audible anyway.



