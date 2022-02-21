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
#define GATE_PIN 3 // PD2

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
    OCR0A = 250;    // This will yield a tick  1000 times per second
    TCCR0A = 1<<WGM01; // CTC
    TCCR0B = (1 << CS01) | (1<< CS00); // divided by 64: 250000 times per second
    TIFR0 = 0;
    TIMSK0 |= 0b10;
}

void dac_write(uint16_t val)
{
    uint8_t dh = 0b00110000 | (val >> 8L);
    uint8_t dl = val & 0xFF;

    PORTB &= !(1<<2);
    SPDR = dh;              
    while(!(SPSR & (1 << SPIF)));
    SPDR = dl;           
    while(!(SPSR & (1 << SPIF)));
    PORTB |= (1<<2);
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
 
    // Convert to 12bit values 
    adcs[index] = (temp<<2L);
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

    // Level will be calculated using 15bits and then sized down to 12. This is
    // so that we can have a better granularity on the rate of change
    uint16_t level = 0;

    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
    dac_write(0xFFF);

    uint16_t ms_count = 0;
    while (1)
    {
        // Debounce gate pin
        uint8_t n = !(PIND & (1<< GATE_PIN));
        if (n != gate_status)
        {
            temp_gate_status = n;
            debounce = 1000L;
        }
        if (debounce >0) debounce--;

        // Check if pin has been steady for a while and its state has changed
        if (debounce == 1 && temp_gate_status != gate_status)
        {
            gate_status = temp_gate_status;

            if (!gate_status) // Gate is released 
            {
                if (state != STATE_IDLE) 
                {
                    state = STATE_RELEASE;
                    ms_count=0;
                }
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
                uint16_t delta_level = 0x7FFFL-level; 
                uint16_t delta_time = (adcs[A_PIN]>ms_count)?adcs[A_PIN]-ms_count:1;
                uint16_t rate = delta_level/delta_time; 
                level += rate;
                if (level >= 0x7FFFL) level=0x7FFFL;
                
                if (ms_count >= adcs[A_PIN] || level == 0x8FFFL) {
                    state = STATE_DECAY;
                    ms_count=0;
                }
            }
            else if (state == STATE_DECAY)
            {
                uint16_t s = adcs[S_PIN] << 3L;
                uint16_t delta_level = (s<level)?level-s:0; 
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
                level = adcs[S_PIN]<<3L; // to 15bit level
                ms_count=0;
            }
            else if (state == STATE_RELEASE)
            {
                uint16_t r = adcs[R_PIN] << 1L; // Multiply range by 2 since we want 0..8192
                uint16_t delta_level = level; 
                uint16_t delta_time = (r>ms_count)?r-ms_count:1;
                if (delta_level == 0) delta_level = 0;
                uint16_t rate = delta_level/delta_time;
                if (level > rate) level-=rate; else level = 0;

                if (ms_count >= r || level ==0)
                {
                    state = STATE_IDLE;
                    ms_count=0;
                }
            }
            else if (state == STATE_IDLE)
            {
                level = 0;
            }

            dac_write(level>>3L);
        }
    }
}
