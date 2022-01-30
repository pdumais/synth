# Modular Synthesizer

# Design
- 5V system
- uses computer PSU
- 2 signal paths:
    1) The audio signal. -5 - +5V. Any digital conversion should happens with 24bit or at least 16 bit
    2) Control Voltages. They work on unipolar 5V. Digital conversions of 10bits.

# Current modules
- dual VCO and quad LFO. Square only
- LFO shaper: to convert square to trianble/sine or anything in between
-

# Known issues
- Power filtering
    The oscilloscope shows that the -5 and +5 rails oscillate at an amplitude of 0.4v. The wave has a period of 500ns.
    That's 2mhz. 
    For the CV path, this gets filtered out by the sampling rate of the ADC, so no need to do anything about it. When CV
    is used with analog, it wouldn't be audible anyway.



