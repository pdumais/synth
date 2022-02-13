#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

struct {
    uint8_t cv_channel[3];
} config;

uint8_t cv_note[3] = {0xFF, 0xFF, 0xFF};

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
}

void handle_note_on(uint8_t chan, uint8_t d2, uint8_t d3)
{
    for (uint8_t i = 0; i < 3; i++) if (config.cv_channel[i] == chan)
    {
        cv_note[i] = d2;
        set_gate(i, 0xFF);
    //TODO: send pitch to DAC
        return;
    }


}

void handle_note_off(uint8_t chan, uint8_t d2)
{
    for (uint8_t i = 0; i < 3; i++) if (config.cv_channel[i] == chan)
    {
        if (cv_note[i] == d2)
        {
            cv_note[i] = 0xFF;
            set_gate(i, 0x00);
            return;
        }

    }
}

void handle_sysex(uint8_t d1, uint8_t d2, uint8_t d3) 
{
    if (d1 == 0x55 && d2==0x66 && d3==0x77) PINF = 1;
}


int main(void)
{

	clock_prescale_set(clock_div_1);

    DDRF=0xFF;
    DDRC=0xFF;
    DDRB=0xFF;
    DDRD=0xFF;

    for (uint8_t i = 0; i < 3; i++) config.cv_channel[i] = i;

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
            //TODO: extract channel and pass down to function
			if (ev_number == MIDI_COMMAND_NOTE_ON)
            {
                handle_note_on(chan_number, ev.Data2, ev.Data3);
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

