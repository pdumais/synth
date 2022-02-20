The first circuit in the diagram can be used to to mix a CV with a potentiometer value and attenuate the result.
This is useful when wanting to use CV and knob at the same time.

The second circuit is used to disable the potentiometer when the CV signal is connected. The resistor is used to protect
the circuit when the jack is being plugged in and the pot is at 0%. There would be a small time when the top would touch
the switch and the signal would flow to the potentiometer.

The third circuit creates an average of both inputs. So the following can happen:
	pot at 0v, CV at 0v: 0v
	pot at 0v, CV at 5v: 2.5v
	pot at 5v, CV at 0v: 2.5v
	pot at 5v, CV at 5v: 5v

So connecting the jack would only allow a partial swing over the base potentiometer value.
It is possible to change the values of the resistors to change the weight of each line. For example, if
the CV resistor is twice as high as the POT, then the POT will have a larger range of operation. it could
then be useful to have 2 pots instead of the resistor and we can control the gain of each input
