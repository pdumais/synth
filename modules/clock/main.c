#define F_CPU            8000000UL
#define BPM_STABLE_COUNT 120
#define MSG_DISPLAY_TIME 100

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>

static uint8_t last_quad = 0x0;
uint16_t EEMEM stored_bpm = 120;
uint16_t current_bpm = 120;
uint16_t bpm_stabilizer = 0;

int8_t quad_matrix[] =  {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};

void timer_init() 
{
    OCR0A = 255; // 61 times per seconds. This is for eeprom debouncing
    TCCR0A = 1<<WGM01; 
    TCCR0B = (1 << CS02) | (1<< CS00); // divided by 1024: 15625 times per second 
    TCNT0=0;

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


void display_value(uint16_t val) 
{
    uint16_t c = val/100L;
    uint16_t u = val%10L;
    uint16_t d = (val-u-(c*100L))/10L;

    send_value_to_display(c, 1);
    send_value_to_display(d, 2);
    send_value_to_display(u, 3);
}

void set_bpm(uint16_t bpm) 
{
    // F_CPU/(1024*ocra) = bpm/60
    // ocra = (f_cpu*60)/(1024*bpm) = 468750/bpm
    // But we want the timer to go 8 times faster, and another 2 times faster for half-cycles pin toggling.
    uint16_t c = 29297L;
    c = c / (uint16_t)bpm;
    OCR1A = c;
    if (TCNT1 > OCR1A) {
        TCNT1 = OCR1A-1;
    }
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

        int8_t v = quad_matrix[quad+(last_quad<<2)];
        if (v != 2)
        {
            if (v == -1) current_bpm--; 
            else if (v == 1) current_bpm++; 

            if (current_bpm > 800) current_bpm = 800;
            if (current_bpm < 30) current_bpm = 30;
            bpm_stabilizer = BPM_STABLE_COUNT;
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
    DDRB = 0b00000111;

    timer_init();

    current_bpm = eeprom_read_word(&stored_bpm);
    if (current_bpm > 800L) current_bpm=120;
    set_bpm(current_bpm);

    uint8_t interrupt_count = 0;
    uint16_t stable_bpm = current_bpm;
    uint16_t msg_display = 0; 
    while (1) {


        if (TIFR1 & (1<<OCF1A)) {
            TIFR1 |= (1<<OCF1A);
            PINB = (1<<1);
            interrupt_count++;
            if (interrupt_count == 8) {
                PINB = (1<<2);
                interrupt_count=0;
                //display_value(current_bpm);
            }
        }

        if (TIFR0 & (1<<OCF0A)) {
            TIFR0 |= (1<<OCF0A);
            if (bpm_stabilizer > 0) bpm_stabilizer--;

            if (msg_display > 0) {
                msg_display--;
                // Message has been displayed long enough
                if (msg_display == 0) {
                    display_value(current_bpm);
                }
            }

            if (stable_bpm != current_bpm && bpm_stabilizer == 0) {
                stable_bpm = current_bpm;
                msg_display = MSG_DISPLAY_TIME;
                send_value_to_display(0, 1);
                send_value_to_display(0, 2);
                send_value_to_display(0, 3);
                eeprom_write_word (&stored_bpm, current_bpm);
            }
        }
        rotary_read();
    }
}
