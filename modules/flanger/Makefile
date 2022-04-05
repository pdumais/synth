SERIALPORT=/dev/ttyUSB0
DEVICE=t44
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
OUTPUTNAME=firmware

CC=avr-gcc
OBJCOPY=avr-objcopy
LDFLAGS=
CFLAGS= -mmcu=attiny44 --save-temps

all: $(OUTPUTNAME).hex

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm -f *.o
	-rm  $(OUTPUTNAME).hex
	-rm  $(OUTPUTNAME).out

%.hex: %.out
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.out: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(CFLAGS) -o $@

install: $(OUTPUTNAME).hex
	# FTDI rx to PA1, tx to PA2
	avrdude -carduino -P $(SERIALPORT) -b19200 -p$(DEVICE)  -U flash:w:$(OUTPUTNAME).hex:i

