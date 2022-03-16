#define F_CPU            8000000UL
#define BPM_STABLE_COUNT 120
#define MSG_DISPLAY_TIME 100
#define MAX_SQ 2
#define ADMUX_VAL (0 << REFS0) | (0 << ADLAR)


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>


struct sq_config {
    uint16_t i;
    uint16_t max;
    uint16_t half;
    uint16_t f;
};

struct sq_config sq[4];
uint16_t rnd1 = 0xACE1;
uint32_t rnd2 = 0xDEADBEEF;


uint8_t get_rnd() {

    uint16_t x = ((rnd1 >> 11L) & 1L) ^ ((rnd1 >> 13L) & 1L) ^ ((rnd1 >> 14L) & 1L);
    uint32_t y = ((rnd2 >> 7L) & 1L) ^ ((rnd2 >> 17L) & 1L) ^ ((rnd2 >> 5L) & 1L);
    rnd1 = (rnd1 << 1L) | x;
    rnd2 = (rnd2 << 1L) | y;

    return x^y;
}

void timer_init() 
{
    // Noise timer
    TCCR0A = 1<<WGM01; 
    TCCR0B = (1 << CS02) | (0<< CS00); // divided by 64 -> 125khz 
    OCR0A = 0; // 62.5khz 
    TCNT0=0;

    // Square wave timer
    TCCR1A = 0;
    TCCR1B = (1<<WGM12) |(0 << CS11) | (1<< CS12); // divided by 256 -> 31250 hz
    OCR1A = 0; 
    TCNT1=0;
}


void adc_init() {
    ADMUX = ADMUX_VAL | 5 ; // AVcc with capacitor to ARef
    ADCSRA |= (1 << ADPS0) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN) | (1<<ADSC);
}


void set_sq_freq(uint8_t i, uint16_t f) {
    sq[i].f = f;
    sq[i].max = 31250L/sq[i].f;
    sq[i].half = sq[i].max >> 1;
    if (sq[i].i >sq[i].max) sq[i].i = sq[i].max;
}


int main(void)
{
    DDRA = 0b11000111; 

    timer_init();
    adc_init();

    sq[0].f = 2005L;
    sq[1].f = 460L; 

    for (uint8_t i = 0; i < MAX_SQ; i ++) {
        set_sq_freq(i, sq[i].f);
    }


    uint8_t rnd_index = 0;
    uint8_t rnd_last = 0;
    while (1) {

        if ((ADCSRA & (1 << ADIF)))
        {
            uint8_t adc = ADMUX & 0b111;
            uint8_t l = ADCL;
            uint8_t h = ADCH;
            ADCSRA |= (1 << ADIF) | (1<<ADSC); // Clear ADIF by writing 1 in it, and start a new conversion

            uint16_t temp = ((h<<8L)|l);

            if (adc == 5) {
                set_sq_freq(0, 500L+temp);
            }
        }



        if (TIFR0 & (1<<OCF0A)) {
            TIFR0 |= (1<<OCF0A);
            
            if (get_rnd()) {
                PINA = 1<<7;
            }
        }


        if (TIFR1 & (1<<OCF1A)) {
            TIFR1 |= (1<<OCF1A);

            uint8_t metal_pin = 0;

            for (uint8_t i = 0; i < MAX_SQ; i ++) {
                sq[i].i--;
                if (sq[i].i == 0) {
                    sq[i].i = sq[i].max;
                } else if (sq[i].i >= sq[i].half) {
                    metal_pin = 1;
                }
            }
            if (metal_pin) {
                PORTA |= (1<<6);
            } else {
                PORTA &= ~(1<<6);
            }

        }
    }
}
