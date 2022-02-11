Based on the as3320.

- The range for cutoff frequency CV is +-6V. What does that relate to?
    18mV per octave. they say to use a range from -25 to 155 
    this is a range of 180mV, so 10 octaves. if we stay above 0, we loose
    the last 1.5 octave since increasing voltage lowers the.cutoff

    the 100k and 1.8k resistors on the cv input in the datasheet are a divider
    to drop 10v down to 180mV. I need to adapt this for 5v

- The range for resonance CV is 0-2V. What does that relate to?
- No need for output buffer
- will output audio signal stay within +-5v?

