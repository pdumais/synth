#define F_CPU            8000000UL
#define ADC_DEBOUNCE_DEPTH 5L
#define ADC_DEBOUNCE_COUNT ((1L<<ADC_DEBOUNCE_DEPTH))


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>



static volatile uint16_t adcs[8];
static volatile uint16_t adcs_temp[8];
static volatile uint16_t adc_debounce[8];

void timer_init() 
{
    TCCR0A = (1<<WGM01); // CTC
    TCCR0B = (1<<CS00)|(1<<CS02);   //prescaling with 1024. Timer freq=7812.5hz
    OCR0A = 3; // Timer = 1953 //3906hz 
    TCNT0=0;

    TCCR1A = (0<<WGM10)|(1<<WGM11)|(1<<COM1A1)|(1<<COM1A0)|(1<<COM1B1);
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS10);   //prescaling with 1. Timer freq=8mhz
    TCNT1=0;
}

void adc_init() {
    for (uint8_t i = 0; i < 8; i++) {
        adcs[i] = 0;
        adc_debounce[i] = ADC_DEBOUNCE_COUNT;
        adcs_temp[i] = 0;
    }

    ADMUX = 0b0; // Aref disabled 
    ADCSRB |= (0 << ADLAR);
    ADCSRA |= (1 << ADPS0) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN) | (1<<ADSC);
}

uint8_t adc_read() {
    if (!(ADCSRA & (1 << ADIF))) return 0;

    uint8_t admux = ADMUX;
    uint8_t index = admux & 0b00111;
    uint8_t l = ADCL;   
    uint8_t h = ADCH;   

    uint16_t temp = ((h<<8L)|l);

    adcs_temp[index] += temp;
    adc_debounce[index]--;
    if (adc_debounce[index] == 0) {
        adcs[index] = adcs_temp[index] >> ADC_DEBOUNCE_DEPTH;
        adcs_temp[index] = 0;
        adc_debounce[index] = ADC_DEBOUNCE_COUNT;
    }             

    // Cycle thru ADC 0,3,4
    if (index == 0) index=3;
    else if (index == 3) index=4;
    else if (index == 4) index=0;
    
    ADMUX = (admux & 0b11100000) | ((index) & 0b00111);
    ADCSRA |= (1 << ADIF) | (1<<ADSC); // Clear ADIF by writing 1 in it, and start a new conversion

    return 1;
}

void set_clock(uint16_t val)
{
    if (val < 4) val = 4;
    ICR1 = val;
    OCR1A = val>>1L;
    OCR1B = val>>1L;
    if ((TCNT1 > ICR1) && TCNT1 != 0) TCNT1=ICR1-1L;
}

int main(void)
{
    PORTB=0;
    DDRA = 0b01100000; 
    DDRB = 0b00000110;


	adc_init();
    timer_init();

    // 15 -> 500khz -> 4ms
    // 800 -> 10Khz
    //set_clock(15L); 

    uint16_t rate = 100;
    uint16_t manual = 0;
    uint16_t depth = 100L;
    uint16_t delay = 15;
    uint16_t dir = 1;
    uint16_t rate_counter = 0;

    while (1) {
        uint16_t temp = delay;

        if (adc_read()) {
            if (adcs[4] != depth) {
                depth = adcs[4];
            }
            if (adcs[3] != rate) {
                rate = adcs[3];
            }
            if (adcs[0] != manual) {
                // The LFO might be active and oscillating around "manual". So we don't
                // wanna overwrite the value. Instead, we wanna take the detla from the current value
                // and the "manual" base, and rebase the delta on the new manual value.
                uint16_t delta = temp - manual;
                manual = adcs[0];
                temp = manual+delta;
                if (temp > 1023L) temp = 1023L;
            }

        }


        uint16_t max = manual + depth;
        if (max > 1023L) max = 1023L;
        if (max == 0) max = 1;


        if (TIFR0 & (1<<OCF0A)) {
            TIFR0 |= (1<<OCF0A); // Clear interupt flag
            rate_counter++;

            if ((rate_counter >= (1023L-rate)) && (rate != 0)) {
                rate_counter = 0;

                // This is a triangle wave that will oscillate between manual and manual+depth
                if (dir == 1) {
                    temp++;
                    if (temp >= max) {
                        // we're about to exceed MAX. so clamp to max and change direction
                        temp = max;
                        dir = 0;
                    }
                } else {
                    temp--;
                    if (temp == 0 || temp <= manual) {
                        // change direction if going too low, below "manual" or 0 
                        dir = 1;
                    }
                }    
            }
        }
    
        if (temp != delay) {
            delay = temp;
            set_clock(delay);
        }
    }
}
