#define F_CPU            8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LED_OFF_TIME     300000L
#define LED_ON_TIME      2000L

uint8_t current_adc;

void adc_init() {
    ADMUX = (0 << REFS0) | 0b111; // AVcc with capacitor to ARef
    ADCSRB |= (1 << ADLAR);
    ADCSRA |= (1 << ADPS0) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN) | (1<<ADSC);
} 

uint8_t adc_read() {
    if (!(ADCSRA & (1 << ADIF))) return current_adc;

    uint8_t l = ADCL;
    uint8_t h = ADCH;
    ADCSRA |= (1 << ADIF) | (1<<ADSC); // Clear ADIF by writing 1 in it, and start a new conversion
    return h;
}

uint8_t spi_write(uint8_t c) {
    USIDR = c;
    USISR=(1<<USIOIF);  // Clear overflow flag by writing 1 to it.   
    while(1) {
        USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
        if ((USISR & (1<<USIOIF)) != 0) break;
    }
    uint8_t ret = USIDR;
    return ret;
}

void digipot_write(uint8_t v) {
    PORTB&=~(1<<PB2);
    spi_write(0);
    spi_write(v);
    PORTB|=(1<<PB2);
}

int main(void)
{
    PORTA=0;
    PORTB=0b101;
    DDRA=(1<<PA5)|(1<<PA4);  
    DDRB=(1 << PB0) | (1 <<PB2);

    adc_init();

    digipot_write(255);

    uint32_t led_flash = 0;
    uint8_t current_adc = 0;
    uint8_t tmp = 0;
    while (1) {
        tmp = adc_read();
        if (tmp != current_adc) {
            current_adc = tmp;
        }

        if (current_adc <200) PORTB|=1; else PORTB &=0xFE;
 /*       led_flash++;
        if (led_flash == LED_OFF_TIME) PORTB&=0xFE; 
        if (led_flash == (LED_ON_TIME+LED_OFF_TIME)) {
            led_flash = 0;
            PORTB|=0x1; 
        }*/

    digipot_write(current_adc);
//    digipot_write(30);

//    PORTB&=~(1<<PB2);
//    spi_write(0);
//    spi_write(64);
//    PORTB|=(1<<PB2);
//        PORTB&=~(0b10);
//        spi_write(0);
//        spi_write(255);
//        PORTB|=0b10;

    }
}
