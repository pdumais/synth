#define F_CPU            8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

static uint8_t last_quad = 0x0;
uint8_t current_bpm = 120;

uint8_t quad_matrix[] =  {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};

void timer_init() 
{
    TCCR1A = 0; 
    TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS12);   //prescaling with 1024. Timer freq=7812.5
    TCNT1=0;
}

void send_value_to_display(uint8_t val, uint8_t display) 
{
    if (val > 9) return;
    val = (val&0x0F)  << 3;

    // Clear current CS
    PORTA &= ~(1<<7);
    PORTA &= ~(1);
    PORTB &= ~(1);

    PORTA &= 0b10000111; //clear old digit
    PORTA |= val;

    // set CS
    if (display == 1) 
    {
        PORTB |= 1;
    }
    if (display == 2) 
    {
        PORTA |= (1<<7);
    }
    if (display == 3) 
    {
        PORTA |= 1;
    }
}


void display_value(uint8_t val) 
{
    uint8_t c = val/100;
    uint8_t u = val%10;
    uint8_t d = (val-u-(c*100))/10;

    send_value_to_display(c, 1);
    send_value_to_display(d, 2);
    send_value_to_display(u, 3);
}

void set_bpm(uint8_t bpm) 
{
    // F_CPU/(1024*ocra) = bpm/60
    // ocra = (f_cpu*60)/(1024*bpm) = 468750/bpm
    // But we want the timer to go 8 times faster, and another 2 times faster for half-cycles pin toggling.
    uint16_t c = 29297L;
    c = c / (uint16_t)bpm;
    OCR1A = c;
    display_value(bpm);
}

void rotary_read() 
{
    uint8_t quad = PINA;
    quad = (quad>>1) & 0x03;

    if (last_quad == 0xFF)
    {
        last_quad = quad;
        return;
    }
    if (last_quad != quad)
    {

        uint8_t v = quad_matrix[quad+(last_quad<<2)];
        if (v != 2)
        {
            current_bpm += v;
            if (current_bpm > 240) current_bpm = 240;
            if (current_bpm < 30) current_bpm = 30;
            set_bpm(current_bpm);
            last_quad = quad;
        }
    }


}

int main(void)
{
    PORTA=0b110;
    PORTB=0;
    DDRA = 0b11111001;
    DDRB = 0b00000110;


    timer_init();
    set_bpm(current_bpm);

    uint8_t interrupt_count = 0;
    while (1) {
        if (TIFR1 & (1<<OCF1A)) {
            TIFR1 |= (1<<OCF1A);
            PINB = (1<<1);
            interrupt_count++;
            if (interrupt_count == 8) {
                PINB = (1<<2);
                interrupt_count=0;
                display_value(current_bpm);
            }
        }

        rotary_read();
    }
}
