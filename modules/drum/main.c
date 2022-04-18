#define F_CPU            8000000UL
#define USART_BAUDRATE   31250
#define BAUD_PRESCALE (F_CPU / USART_BAUDRATE) // 256
#define DIVISOR             8
#define PRESCALE            2
#define FULL_BIT_TICKS      ( CYCLES_PER_BIT / DIVISOR )

#define BPM_STABLE_COUNT 120
#define MSG_DISPLAY_TIME 100
#define MAX_SQ 2
#define ADMUX_VAL (0 << REFS0) | (0 << ADLAR)

#define TRIGGER_TOP 4000L 
#define TRIGGER_THRESH (TRIGGER_TOP-500L) 
#define MAX_VOICE 5

#include "uart.h"
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
uint16_t voice_trigger[MAX_VOICE] ;
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
    // Square wave timer
    TCCR1A = 0;
    //TCCR1B = (1<<WGM12) |(0 << CS11) | (1<< CS12); // divided by 256 -> 31250 hz
    TCCR1B = (1<<WGM12) |(0 << CS11) | (1<< CS10); // divided by 64 -> 125khz
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


void midi_init() {
    PORTA |= (1<<6); // Enable pull-up on RX pin
    uart_init();
}


void trigger_voice(uint8_t voice) {
    if (voice >= MAX_VOICE) return;
    voice_trigger[voice] = TRIGGER_TOP;
}

void process_triggers() {
    for (uint8_t i = 0; i < MAX_VOICE; i++) {
        if (voice_trigger[i] > 0) {
            voice_trigger[i]--;
        }
    }

    if (voice_trigger[0] > TRIGGER_THRESH) {
        PORTB |= 1;
    } else {
        PORTB &= 0xFE;
    }

    if (voice_trigger[1] > TRIGGER_THRESH) {
        PORTB |= 2;
    } else {
        PORTB &= 0xFD;
    }

    if (voice_trigger[2] > TRIGGER_THRESH) {
        PORTB |= 4;
    } else {
        PORTB &= 0xFB;
    }

    if (voice_trigger[3] > TRIGGER_THRESH) {
        PORTA |= 1;
        DDRA |= 1; 
    } else {
        PORTA &= 0xFE;
        DDRA &= 0xFE; // Leave pin floating, so we dont interfere with button
    }
    if (voice_trigger[4] > TRIGGER_THRESH) {
        PORTA |= (1<<3);
        DDRA |= (1<<3); 
    } else {
        PORTA &= ~(1<<3);
        DDRA &= ~(1<<3); // Leave pin floating, so we dont interfere with button
    }
}

int main(void)
{
    DDRA = 0b10100111; 
    DDRB = 0xFF;

    midi_init();
    timer_init();
    //adc_init();

    sq[0].f = 8000L;
    sq[1].f = 6010L; 

    for (uint8_t i = 0; i < MAX_SQ; i ++) {
        set_sq_freq(i, sq[i].f);
    }


    uint8_t rnd_index = 0;
    uint8_t rnd_last = 0;

    uint8_t metal_pin = 0;

    for (uint8_t i = 0; i < MAX_VOICE; i++) voice_trigger[i] = 0;

    sei();
    uint8_t c;
    uint8_t midi_status = 0;
    uint8_t midi_d1=0;
    uint16_t noise_pin = 0;

    for (uint8_t i = 0; i < MAX_VOICE; i++) voice_trigger[i] = 0;

    while (1) {
        if (uart_getc(&c) == 1) {
            if (c&0x80) {
                if ((c>>4) == 0x09) midi_status = 0x90; else midi_status = 0;
                midi_d1 = 0;
            } else if (midi_status == 0x90) {
                if (midi_d1) {
                    if (c != 0) {
                        uint8_t n = midi_d1 % 12;
                        trigger_voice(n);
                    }

                    midi_d1 = 0;
                } else {
                    midi_d1 = c;
                }
            }


        }

        if (TIFR1 & (1<<OCF1A)) {
            TIFR1 = (1<<OCF1A);

            noise_pin = (noise_pin+1)&0b111;
            if (get_rnd() && ((PINA&(1<<4)) || (noise_pin == 0))) {
                PINA = (1<<5);
            }

            process_triggers();
    
            metal_pin = 0;
            for (uint8_t i = 0; i < MAX_SQ; i ++) {
                sq[i].i--;
                if (sq[i].i == 0) {
                    sq[i].i = sq[i].max;
                } else if (sq[i].i >= sq[i].half) {
                    metal_pin = 1;
                }
            }
            if (metal_pin) {
                PORTA |= (1<<7);
            } else {
                PORTA &= ~(1<<7);
            }

        }
    }
}
