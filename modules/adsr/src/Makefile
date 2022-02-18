SERIALPORT=/dev/tyUSB9
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
MCU=$(DEVICE)
OUTPUTNAME=firmware

CC=avr-gcc
OBJCOPY=avr-objcopy
LDFLAGS=
CFLAGS= -mmcu=atmega328p

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
	avrdude -carduino -P $(SERIALPORT) -b57600 -pm328p  -U flash:w:$(OUTPUTNAME).hex:i

