#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

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


void send_adc_data(uint8_t cs, uint16_t data)
{
    PORTF = ~cs;
    //TODO:  send data thru SPI. Need to send async ? but then I need to create a queue
    PORTF = 0xFF;
}

void set_12bit_adc(uint8_t adc, uint16_t val)
{
    uint8_t cs = 0;
    if (adc == 0) cs = 1 <<4;
    else if (adc == 1) cs = 1 <<1;
    else if (adc == 2) cs = 1 <<0;
    else return;

    val &= 0x0FFFL; 
    val |= (0b0011) << 12L;
    send_adc_data(cs, val);

}

void set_8bit_adc(uint8_t adc, uint8_t subdev, uint8_t val)
{
    uint8_t cs = 0;
    if (adc == 0) cs = 1 <<7;
    else if (adc == 1) cs = 1 <<6;
    else if (adc == 2) cs = 1 <<5;
    else return;

    uint16_t val16 = val;
    val16 = val16 << 4L;

    if (subdev == 1) val16 |= 0x8000;
    val16 |= (0b0011) << 12L;
    send_adc_data(cs, val16);
}

uint16_t get_pitch(uint8_t note)
{
    //TODO: use same mapping than oscillator
    return 0;
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
        set_8bit_adc(i, 1, d3);
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
        set_8bit_adc(i, 0,  d3); // Set level
        set_12bit_adc(i, pitch); // Set pitch
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
            set_8bit_adc(i, 0, 0); // Set level
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


int main(void)
{

	clock_prescale_set(clock_div_1);

    PORTF = 0xFF;
    DDRF=0xFF;
    DDRC=0xFF;
    DDRB=0xFF;
    DDRD=0xFF;


    load_config();

	USB_Init();
	GlobalInterruptEnable();

	MIDI_EventPacket_t ev;
    memset(&ev, 0, sizeof(MIDI_EventPacket_t));
    while (true)	
	{
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

