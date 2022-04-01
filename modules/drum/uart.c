#define F_CPU 8000000UL

#include "uart.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#define BAUD 31250
#define TIMER_TICK ((float) (F_CPU/8)/BAUD)

static uint8_t buf[8];
static volatile uint8_t bufin = 0;
static uint8_t  bufout = 7;

uint8_t brev(uint8_t x)
{
    x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
    x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
    x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
    return x;
}

void uart_init()
{
	DDRA &= ~(1<<6);

    TCCR0A = 1<<WGM01;              // CTC mode
    TCCR0B = 1<<CS01; 

	GIFR = (1<<PCIF0);
	PCMSK0 |= (1<<PCINT6);
	GIMSK |= (1<<PCIE0);

	OCR0A = (uint8_t)(TIMER_TICK + (TIMER_TICK/2));
}

uint8_t uart_getc(char *dst)
{
	uint8_t temp;

	temp = (bufout + 1) & 0b111;
	if (temp == bufin) return 0;

	//Get data from buffer and bitswap
	*dst = brev(buf[temp]);

	bufout = temp;
	return 1;
}


ISR(PCINT0_vect)
{
	//Only react if Pin is low (startbit)
	if (PINA & (1<<6)) return;
	PCMSK0 = 0;

	TCNT0 = 0;
	OCR0A = (uint8_t)(TIMER_TICK + TIMER_TICK/2);

	TIFR0 = (1<<OCF0A);
	TIMSK0 = (1<<OCIE0A);
	USISR = (1<<USIOIF) | 8;
	USICR |= (1<<USICS0) | (1<<USIOIE);

}

ISR(TIM0_COMPA_vect)
{
	OCR0A = (uint8_t)TIMER_TICK;
	TIMSK0 = 0;
}

ISR(USI_OVF_vect)
{
	uint8_t temp;
	uint8_t data;

    USICR = 0;
	data = USIDR;
	temp = (bufin + 1) & 0b111;
	buf[bufin] = data;
	bufin = temp;
	PCMSK0 |= (1<<PCINT6);
}
