#include <avr/io.h>
 
#define DELAY 5000
#define F_OSC 16000000

// Atack: Time from 0 to 5v output. Time range: 0..4096ms
// Decay: Time from 5v to sustain level. Time range: 0..4096ms
// Sustain: Level from 0 to 5V. This will map 1:! with the S input
// Release: Time from sustain level to 0V. Time range 0..8192ms

#define A_PIN 0
#define D_PIN 1
#define S_PIN 2
#define R_PIN 3
#define GATE_PIN 2 // PD2

#define STATE_IDLE 0
#define STATE_ATTACK 1
#define STATE_DECAY 2
#define STATE_SUSTAIN 3
#define STATE_RELEASE 4

uint16_t adcs[5];

void spi_init()
{
}

void timer_init()
{
    TCNT0 = 0;
    OCR0A = 250;	// This will yield a tick  1000 times per second
    TCCR0A = 1<<WGM01; // CTC
    TCCR0B = (1 << CS01) | (1<< CS00); // divided by 64: 250000 times per second
    TIFR0 = 0;
    TIMSK0 |= 0b10;
}

void dac_write(uint16_t val)
{
}

void adc_init() {
    ADMUX = (1 << REFS0) | (0 << ADLAR); // AVcc with capacitor to ARef
    ADCSRA |= (1 << ADPS0) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN) | (1<<ADSC);
}
 
void adc_read() {
    if (!(ADCSRA & (1 << ADIF))) return;
 
    uint8_t admux = ADMUX;
    uint8_t index = admux & 0b00111;
    uint8_t l = ADCL;
    uint8_t h = ADCH;
    ADMUX = (admux & 0b11100000) | ((index+1) & 0b00011);
    ADCSRA |= (1 << ADIF) | (1<<ADSC); // Clear ADIF by writing 1 in it, and start a new conversion
    uint16_t temp = ((h<<8L)|l);
 
	// Those values are 10bit but we will use them in the 12bitDAC. So convert that to 12bit and extrapolate
    adcs[index] = (temp<<2L)|0b10L;
}

int main (void)
{
    DDRD = 0x00;
    DDRC = 0x00;
    DDRB = (1<<3)|(1<<5)|(1<<2); // SPI

    timer_init();
    spi_init();
    adc_init();

    uint8_t state= STATE_IDLE;
    uint8_t gate_status = 0;
    uint8_t temp_gate_status = 0;
    uint16_t debounce=0;
    uint16_t level = 0;

	uint16_t ms_count = 0;
    while (1)
    {
        // Debounce gate pin
        uint8_t n = (PORTD & (1<< GATE_PIN));
        if (n != gate_status)
        {
            temp_gate_status = gate_status;
            debounce = 1000;
        }
        if (debounce >0) debounce--;

        // Check if pin has been steady for a while and its state has changed
        if (debounce == 0 && gate_status != temp_gate_status)
        {
            gate_status = temp_gate_status;

            if (!gate_status) // Gate is released 
            {
                if (state != STATE_IDLE) state = STATE_RELEASE;
            }
            else
            {
                // Move to Attack state
				state = STATE_ATTACK;
				ms_count = 0;
            }
        }

		adc_read();

		if (TIFR0 & (1<<OCF0A)) // This will be true every 1ms
		{
			TIFR0 |= (1<<OCF0A);
			ms_count++;
	        if (state == STATE_ATTACK)
	        {
				uint16_t delta_level = 4095L-level; 
				uint16_t delta_time = (adcs[A_PIN]>ms_count)?adcs[A_PIN]-ms_count:1;
				uint16_t rate = delta_level/delta_time; 
	            if ((level+rate) > 4096) level=4096; else level+=rate;
				
				if (ms_count >= adcs[A_PIN] || level == 4096) {
					state = STATE_DECAY;
					ms_count=0;
				}
	        }
	        else if (state == STATE_DECAY)
	        {
				uint16_t delta_level = (adcs[S_PIN]<level)?level-adcs[S_PIN]:0; 
				uint16_t delta_time = (adcs[D_PIN]>ms_count)?adcs[D_PIN]-ms_count:1;
				uint16_t rate = delta_level/delta_time; 
	            if (level > rate) level-=rate; else level = 0;
	
				if (ms_count >= adcs[D_PIN]) {
					state = STATE_SUSTAIN;
					ms_count=0;
				}
	        }
	        else if (state == STATE_SUSTAIN)
	        {
	            level = adcs[S_PIN];
	        }
	        else if (state == STATE_RELEASE)
	        {
	            uint16_t r = adcs[R_PIN] << 1L; // Multiply range by 2 since we want 0..8192
				uint16_t delta_level = level; 
				uint16_t delta_time = (r>ms_count)?r-ms_count:1;
	            if (delta_level == 0) delta_level = 0;
				uint16_t rate = delta_level/delta_time; 
	            if (level > rate) level-=rate; else level = 0;
	
				if (ms_count >= adcs[R_PIN] || level ==0)
	            {
					state = STATE_IDLE;
					ms_count=0;
				}
	        }
	        else if (state == STATE_IDLE)
	        {
				level = 0;
	        }
	
			dac_write(level);
		}
    }
}
