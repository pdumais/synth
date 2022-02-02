#Design
This is a 8 step sequencer based on the 4017 counter. It is designed to make it possible to daisy-chain 2 of those to get a 16 step sequencer

The 4017's Q1 to Q8 outputs go through 8 optentiometers which are all connected to unity gain opamp. The opamp is used to limit the current draw from the system after going out on the CV output

I had a lot of help from this thread: https://www.modwiggler.com/forum/viewtopic.php?t=258437
I'm not sure why having all pots connected together with a 100K resistor doesn't create a voltage divider. It used to create
on and it was a problem. But by connecting everything the the inverting amplifier at the end, it solved that problem. 
At the time of writing this, I don't understand why.

## 4163 binary counter
The 4163 (4 bit binary counter) is used to control the daisy chaining. QD will be low for the first 8 clocks and then high 
for the next 8 clocks. This is used to control which of the 2 sequencers are active when daisy chained. there are 6 pins that 
require a jumper to control the daisy-chain operation 
    - A jumper between pin 1&2 will make the 4017 active  (by controlling the vcc line) when 4163.QD is high, 
      so during steps 9-16. This is the setting required for the second sequencer
    - A jumper between pin 3&4 will make the 4017 active  (by controlling the vcc line) when 4163.QD is 
      low, so during steps 1-8. This is the setting required for the first sequencer
    - A jumper between pin 5&6 will make the 4017 active  (by controlling the vcc line) all the time, 
      bypassing the 4163 logic. This is the setting required when running a standalone sequencer (8 steps only)
 
Note that both sequencers run their own 4163, so it is possible to configure them in a conflicting manner.

When daisy-chaining, a cable needs to be connected between both sequencers so that:
    - the reset line is connected between both sequencers so that pressing the reset button will act on both sequencers
    - the enable line is connected between both sequencers so that pressing the start/stop button will act on both sequencers


## Gate / trigger
TODO

## Inputs / Outputs
## Inputs
    - Clock 
    - enable (for daisy chain)
    - reset (for daisy chain)
    - pause/resume button
    - Buttons to shorten the sequence to 2 or 4 steps. 
    - Power: +12, -12, +5, Ground 
## Outputs
    - 8 pins for potentiometers wipers, 8 pins for their input voltage from the 4017, 1 pin for their ground
    - enable (for daisy chain)
    - reset (for daisy chain)
    - CV
    - Gate
