#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

#define F_CPU 16000000L
#define USART_BAUDRATE 31250
#define BAUD_PRESCALE ((( F_CPU / ( USART_BAUDRATE * 16UL))) - 1)


struct {
    struct {
        uint8_t channel;
        uint8_t current_note;
    } cv[3];
    struct {
        uint8_t channel;
        uint8_t id;
        uint8_t current_value;
    } cc[3];
    uint8_t clock_pw;
} config;


USB_ClassInfo_MIDI_Device_t Keyboard_MIDI_Interface =
	{
		.Config =
			{
				.StreamingInterfaceNumber = INTERFACE_ID_AudioStream,
				.DataINEndpoint           =
					{
						.Address          = MIDI_STREAM_IN_EPADDR,
						.Size             = MIDI_STREAM_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint          =
					{
						.Address          = MIDI_STREAM_OUT_EPADDR,
						.Size             = MIDI_STREAM_EPSIZE,
						.Banks            = 1,
					},
			},
	};


void uart_init()
{
    unsigned int baud = BAUD_PRESCALE;
    UBRR1H = (unsigned char)(baud>>8);
    UBRR1L = (unsigned char)baud;
    UCSR1B = (1<< RXEN1)|(1<< TXEN1);
    UCSR1C = (1<< USBS1)|(3<< UCSZ10);

}

void send_dac_data(uint8_t cs, uint8_t dh, uint8_t dl)
{
    //TODO: Maybe this should be done asynchronously with a queue
    PORTF = ~cs;
    SPDR = dh; 
    while(!(SPSR & (1 << SPIF)));
    SPDR = dl; 
    while(!(SPSR & (1 << SPIF)));
    PORTF = 0xFF;
}

void set_12bit_dac(uint8_t dac, uint16_t val)
{
    uint8_t cs = 0;
    if (dac == 0) cs = 1 <<7;
    else if (dac == 1) cs = 1 <<6;
    else if (dac == 2) cs = 1 <<5;
    else return;

    uint8_t dh = 0b00110000 | (val >> 8L);
    uint8_t dl = val & 0xFF;
    send_dac_data(cs, dh, dl);

}

void set_8bit_dac(uint8_t dac, uint8_t subdev, uint8_t val)
{
    uint8_t cs = 0;
    if (dac == 0) cs = 1 <<4;
    else if (dac == 1) cs = 1 <<1;
    else if (dac == 2) cs = 1 <<0;
    else return;

    uint8_t dh = 0b00110000 | (val >> 4);
    uint8_t dl = val << 4;
    if (subdev == 1) dh |= 0x80;
    send_dac_data(cs, dh, dl);
}

uint16_t get_pitch(uint8_t note)
{
    //TODO: use same mapping than oscillator
    return 0xFFF;
}

void set_gate(uint8_t gate, uint8_t val)
{

    if (gate == 0 && val) PORTC |= (1<<7);
    else if (gate == 0 && !val) PORTC &= ~(1<<7);
    else if (gate == 1 && val) PORTD |= (1<<6);
    else if (gate == 1 && !val) PORTD &= ~(1<<6);
    else if (gate == 2 && val) PORTB |= (1<<7);
    else if (gate == 2 && !val) PORTB &= ~(1<<7);
}

void handle_cc(uint8_t chan, uint8_t d2, uint8_t d3)
{
    for (uint8_t i = 0; i < 3; i++) if (config.cc[i].channel == chan && config.cc[i].id == d2)
    {
        // The range is 0..127, let's double it so we can get the full 5v range
        d3 = d3 <<1;
        set_8bit_dac(i, 1, d3);
        return;
    }
}

void handle_note_on(uint8_t chan, uint8_t d2, uint8_t d3)
{
    for (uint8_t i = 0; i < 3; i++) if (config.cv[i].channel == chan)
    {
        uint16_t pitch = get_pitch(d2);
        config.cv[i].current_note  = d2;
        set_gate(i, 0xFF); // Set Gate
        // The range is 0..127, let's double it so we can get the full 5v range
        set_8bit_dac(i, 0,  d3 << 1); // Set level
        set_12bit_dac(i, pitch); // Set pitch
        return;
    }


}

void handle_note_off(uint8_t chan, uint8_t d2)
{
    for (uint8_t i = 0; i < 3; i++) if (config.cv[i].channel == chan)
    {
        if (config.cv[i].current_note == d2)
        {
            config.cv[i].current_note = 0xFF;
            set_gate(i, 0x00);
            set_8bit_dac(i, 0, 0); // Set level
            set_12bit_dac(i, 0); // Set pitch
            return;
        }

    }
}

void load_config()
{
    // Set default config
    config.clock_pw = 5;
    for (uint8_t i = 0; i < 3; i++) 
    {
        config.cv[i].channel = i;
        config.cc[i].channel = 0;
        config.cc[i].id = i;
    }

    //TODO: load config from eeprom
}

void save_config()
{
    //TODO: save to eeprom
}

void handle_sysex(uint8_t d1, uint8_t d2, uint8_t d3) 
{
    if (d1 == 0x10 && d2 < 3) // Set channel for note CV
    {
        config.cv[d2].channel = d3;
    }
    if (d1 == 0x20 && d2 < 3) // Set channel for cc CV
    {
        config.cc[d2].channel = d3;
    }
    if (d1 == 0x30 && d2 < 3) // Set control ID for cc CV
    {
        config.cc[d2].id = d3;
    }
    if (d1 == 0x40 && d2 >= 5) // Set clock pulse width: 5 .. 255 ms
    {
        config.clock_pw = d2;
    }

    if (d1 == 0x55 && d2==0x55 && d3==0x55) {
        save_config();
    }
}

uint8_t check_serial_midi(uint8_t* buf, uint8_t i)
{
    uint8_t cmd = buf[0]&0xF0;

    // Only read noteon, noteoff and cc
    if (cmd != 0x90 && cmd != 0x80 && cmd != 0xC0) return 0;

    // Command is complete
    uint8_t chan = buf[0]&0x0F;
    if (cmd == 0x90 && i == 3)
    {
        if (buf[2] == 0)
        {
            handle_note_off(chan, buf[1]);
        }
        else
        {
            handle_note_on(chan, buf[1], buf[2]);
        }
        return 0;
    }
    else if (cmd == 0x80 && i == 2)
    {
        handle_note_off(chan, buf[1]);
        return 0;
    }
    else if (cmd == 0xC0 && i == 3)
    {
        handle_cc(chan, buf[1], buf[2]);
        return 0;
    }

    return i;
}

int main(void)
{

	clock_prescale_set(clock_div_1);

    PORTF = 0xFF;
    DDRF=0xFF;
    DDRC=0xFF;
    DDRB=0xFF;
    DDRD=0x0b11111110;

    // Init SPI
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);


    load_config();

	USB_Init();
    uart_init();
	GlobalInterruptEnable();

	MIDI_EventPacket_t ev;
    memset(&ev, 0, sizeof(MIDI_EventPacket_t));
    uint8_t uartbuf[8];
    uint8_t uart_index = 0;
    while (true)	
	{
        while ((UCSR1A & ( 1 << RXC1)))
        {
            uint8_t c = UDR1;
            uartbuf[uart_index] = c;

            uart_index = (uart_index+1)&0b111;
            uart_index = check_serial_midi((uint8_t*)&uartbuf[0], uart_index);

        }

		while (MIDI_Device_ReceiveEventPacket(&Keyboard_MIDI_Interface, &ev))
		{
            uint8_t ev_number = ev.Data1 & 0xF0; 
            uint8_t chan_number = ev.Data1 & 0x0F;
			if (ev_number == MIDI_COMMAND_NOTE_ON)
            {
                if (ev.Data3 == 0) 
                {
                    handle_note_off(chan_number, ev.Data2);
                }
                else
                {
                    handle_note_on(chan_number, ev.Data2, ev.Data3);
                }
            }
			else if (ev_number == MIDI_COMMAND_CONTROL_CHANGE)
            {
                handle_cc(chan_number, ev.Data2, ev.Data3);
            }
			else if (ev.Event == MIDI_EVENT(0, MIDI_COMMAND_SYSEX_1BYTE) ||
		             ev.Event == MIDI_EVENT(0, MIDI_COMMAND_SYSEX_2BYTE) ||
		             ev.Event == MIDI_EVENT(0, MIDI_COMMAND_SYSEX_3BYTE) ||
		             ev.Event == MIDI_EVENT(0, MIDI_COMMAND_SYSEX_START_3BYTE))
            
            {
                handle_sysex(ev.Data1, ev.Data2, ev.Data3);
            }
			else if (ev_number == MIDI_COMMAND_NOTE_OFF)
            {
                handle_note_off(chan_number, ev.Data2);
            }
            else
            {
                PIND = 1;
            }
            memset(&ev, 0, sizeof(MIDI_EventPacket_t));
		}

		MIDI_Device_USBTask(&Keyboard_MIDI_Interface);
		USB_USBTask();
	}
}

void EVENT_USB_Device_Connect(void)
{
}

void EVENT_USB_Device_Disconnect(void)
{
}

void EVENT_USB_Device_ConfigurationChanged(void)
{
	MIDI_Device_ConfigureEndpoints(&Keyboard_MIDI_Interface);
}

void EVENT_USB_Device_ControlRequest(void)
{
	MIDI_Device_ProcessControlRequest(&Keyboard_MIDI_Interface);
}

