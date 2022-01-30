SERIALPORT=/dev/ttyUSB9
DEVICE=m1284p
SOURCES=main.c timer.c
OBJECTS=$(SOURCES:.c=.o)
OUTPUTNAME=firmware

CC=avr-gcc
OBJCOPY=avr-objcopy
LDFLAGS=
CFLAGS= -mmcu=atmega1284p --save-temps

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
	avrdude -carduino -P $(SERIALPORT) -b115200 -p$(DEVICE)  -U flash:w:$(OUTPUTNAME).hex:i

