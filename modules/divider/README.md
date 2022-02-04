# Divider
This modules takes a clock input and outputs a version of the same clock divided by 8. So 8hz becomes 1hz.
Square wave only. 5v

There is also a Gate output that can be configured with a different pulse width. The jumpers on on 14pin headers
can be connected in a way to generate a 1/8 pulse width, 2/8 pulse width ... to 7/8 pulse width.
For example, if 6/8 is selected and an input clock of 8hz is fed in, the gate will output a 1hz clock
where the first 6/8 of the cycle will be high, and the last 2/8 will be low.

