#Design
This is a 8 step sequencer based on the 4017 counter. It is designed to make it possible to daisy-chain 2 of those to get a 16 step sequencer

The 4017's Q1 to Q8 outputs go through 8 optentiometers which are all connected to unity gain opamp. The opamp is used to limit the current draw from the system after going out on the CV output

I had a lot of help from this thread: https://www.modwiggler.com/forum/viewtopic.php?t=258437
I'm not sure why having all pots connected together with a 100K resistor doesn't create a voltage divider. It used to create
on and it was a problem. But by connecting everything the the inverting amplifier at the end, it solved that problem. 
At the time of writing this, I don't understand why.

What I didn't like about popular designs that I saw, was that they were all using diodes. The voltage drop of the diode would 
affect the CV by a great margin IMO. With my design, I don't get any voltage drops so CV should be quite accurate. 

## Keeping it simple
As I was designing this module, I realized how many more features I could add to it but it would become increasingly difficult and
more expensive to do so. I realized that I would be much better off to build it with a microcontroller instead. But I decided
to keep using the 4017 because it was an interesting design. 
These are the things I would've liked to do, but decided to hold off until I build a full digital sequencer. The following list 
is more like personal notes so that I can remember later why I didn't implement those.
- A pause button: This sounds simple because all I have to do is inhibit the clock on the 4017 and on the 4163. But both chips
  expect a high and low signal respectively. So I would need a NOT gate for one of them. But then I also need to inhibit the clock
  out of the gate and actually keep that signal high. Instead of using a NOT gate on one of the ic, I could simply cut the clock source.
  but then I would need to tie both ic's clk pins to ground. If I leave it high, I risk creating a rising edge so the pause would 
  force a move to the next step and then pause. So I need to ground the clock signal for the whole circuit, but raise the gate signal.
  it's getting a bit complicated
- The Gate output is just the clock. So it means that the gate will be on at a 50% duty cycle. When feeding into an ADSR, it would
  be disreable to get a better control over the gate length. The way to do this, in my system, is to take the 1/8th output of the 
  [Clock](/modules/clock) and feed it through the [Clock Divider](modules/divider) to adjust the width. Then the Gate can be taken from there
  instead of the sequencer.
- Randomizer:To be able to randomly shuffle steps. That's just not something you can do with the 4017.
- variable step length: it would be possible to create the circuitry to manipulate the clock entering the 4017 and multiply/divide 
  for every step. But that's a lot more components to add and becomes more expensive. This is something that is just easier with a 
  microcontroller

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


## Gate
The gate signal is simply the clock signal being relayed. So a 50% on time.

## Inputs / Outputs
### Inputs
    - Clock in
    - Buttons to shorten the sequence to 2 or 4 steps. 
    - Power: +12, -12, +5, Ground 
    - 8 pins for potentiometers wipers, 8 pins for their input voltage from the 4017, 1 pin for their ground
### Outputs
    - daisy-chain connector
    - CV
    - Gate


## How to use
TODO:
    connect clk barrel and pause button to the input header
    connect a on-mom push button to the "reset button" header. Or use a jumper to leave out the reset function
    If not daisy-chaining to a 2nd sequencer, set jumper on pin 1&2 of "split select"
    If a daisy chain is needed, connect the "daisy chain" header to the "input" header of the 2nd sequencer
    TODO: connection of counter selection
