
The board is programmed through the serial port pins with an FTDI cable. Not through the USB port
    There is a FTDI header on the shield

To test with vkeybd:
    vkeybd --addr 24:0
    The address is the combination of client:port as per /proc/asound/seq/clients
    Testing sysex: amidi -p hw:2,0,0 -S "F0 00 00 <d1> <d2> <d3> F7"


software
    sysex message to confirgure CV assignment. Store/reloaded from eeprom
        config notes: <cv_number 0..2> <channel_to_use> 
        config cc: <cv_number 3..5> <channel_to_use> <cc_number>
        config clock pulse width: 0x80 <pulse_width_ms 1..255>


hardware
    panel-mount din5
    ftdi programmer
