#define F_CPU            8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void timer_init() 
{
//    TCCR1A = (1<<WGM10)|(1<<WGM11)|(1<<COM1B1)|(1<<COM1B0)|(1<<COM1A1);
    TCCR1A = (0<<WGM10)|(1<<WGM11)|(1<<COM1A1)|(1<<COM1A0)|(1<<COM1B1);
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS10);   //prescaling with 1. Timer freq=8mhz
    TCNT1=0;
}

void set_clock(uint16_t val)
{
    TCNT1=0;
    ICR1 = val;
    OCR1A = val>>1L;
    OCR1B = val>>1L;
}

int main(void)
{
    PORTB=0;
    DDRA = 0xFF; 
    DDRB = 0b00000110;

    timer_init();

    // 15 -> 500khz
    // 800 -> 10Khz
    //set_clock(800L); 

    uint16_t i= 15L;
    uint8_t d = 0;
    while (1) {
        if (d == 0) i++;
        if (d == 1) i--;
        if (i==800) d = 1;
        if (i==15) d = 0;
        set_clock(i); 
        for (uint32_t i = 0; i< 5000L; i++);
    }
}
