#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define F_CPU 16000000L

#define LFO1_FREQ 0
#define LFO2_FREQ 1
#define LFO3_FREQ 2
#define LFO4_FREQ 3

#define ADC_DEBOUNCE_DEPTH 0L
#define ADC_DEBOUNCE_COUNT ((1L<<ADC_DEBOUNCE_DEPTH))

static volatile uint16_t freqs[6];
static volatile uint16_t adcs[8];
static volatile uint16_t adcs_temp[8];
static volatile uint16_t adc_debounce[8];

uint16_t lfo_vc_to_count(uint16_t vc)
{
    if (vc <= 25L) vc=25L; // This will prevent from droping below 0.5hz. And also prevent from div0
    // f = (20*vc)/1024 = 16M/(1024*OCRA*counter)
    //   counter = 16M/(20*vc*ocra). and ocra=31.
    //   counter = 25806/vc. But we need to run twice as fast because we wanna toggle square at mid point.
    //   so counter = 12904/vc
    return 12904L/vc;


}

void reset_counter(uint8_t i) {
    set_counter(i, freqs[i]);
}

void adc_init() {
    ADMUX = (1 << REFS0) | (0 << ADLAR); // AVcc with capacitor to ARef
    ADCSRA |= (1 << ADPS0) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN) | (1<<ADSC);

    for (uint8_t i = 0; i <8; i++) {
        adcs_temp[i] = 0;
        adc_debounce[i] = ADC_DEBOUNCE_COUNT;
    }

}

uint8_t adc_read() {
    if (!(ADCSRA & (1 << ADIF))) return 0xFF;

    uint8_t admux = ADMUX;
    uint8_t index = admux & 0b00111;
    uint8_t l = ADCL;
    uint8_t h = ADCH;
    ADMUX = (admux & 0b11100000) | ((index +1) & 0b00111);
    ADCSRA |= (1 << ADIF) | (1<<ADSC); // Clear ADIF by writing 1 in it, and start a new conversion
    uint16_t temp = ((h<<8L)|l);

    
// By debouncing the ADC value, we get a more accurate value that doesn't bounce around so the frequency is more stable
// but it really changes the sound and makes it sound more "digital". By disabling debouncing (0L), we
// get a nice analog-like sound. If enabling debouncing, a value of 4L works good (max is 6L). This would 
// create an average of 16 samples. 
#if ADC_DEBOUNCE_DEPTH == 0L
    if (adcs[index] == temp) return 0xFF;
    adcs[index] = temp; 
    return index;
#else
    adcs_temp[index] += temp;
    adc_debounce[index]--;
    if (adc_debounce[index] == 0) {
            adcs[index] = adcs_temp[index] >> ADC_DEBOUNCE_DEPTH;
            adcs_temp[index] = 0;
            adc_debounce[index] = ADC_DEBOUNCE_COUNT;
            return index;
    }
    return 0xFF;
#endif

}

int main(void)
{
    PORTB = 0x0;
    DDRB = 0xFF;
    DDRD = 0xFF;


    freqs[LFO1_FREQ] = lfo_vc_to_count(250);
    freqs[LFO2_FREQ] = lfo_vc_to_count(250);
    freqs[LFO3_FREQ] = lfo_vc_to_count(250);
    freqs[LFO4_FREQ] = lfo_vc_to_count(250);

    reset_counter(LFO1_FREQ);
    reset_counter(LFO2_FREQ);
    reset_counter(LFO3_FREQ);
    reset_counter(LFO4_FREQ);


    timer_init();
    adc_init();
    sei();
    while (1) {

        if (!get_counter(LFO1_FREQ)) {
            PINB = 8;
            reset_counter(LFO1_FREQ);
        }
        if (!get_counter(LFO2_FREQ)) {
            PINB = 16;
            reset_counter(LFO2_FREQ);
        }
        if (!get_counter(LFO3_FREQ)) {
            PINB = 32;
            reset_counter(LFO3_FREQ);
        }
        if (!get_counter(LFO4_FREQ)) {
            PINB = 64;
            reset_counter(LFO4_FREQ);
        }

        uint8_t index = adc_read();
        if (index >= 0 && index <=3 ) {
            uint16_t d1 = adcs[1];
            uint16_t d2 = adcs[3];
            set_pwm_freq(adcs[0], d1, adcs[2] , d2);
        }

        if (index >= 4 && index <=7)
        {
            uint8_t i = index-4;
            uint16_t adc_val = adcs[index];           
             
            uint16_t new = lfo_vc_to_count(adc_val);
            uint16_t old = freqs[i];

            if (old != new) {
                freqs[i] = new; 
                //reset_counter(i);
            }
        }

        check();
    }
}
