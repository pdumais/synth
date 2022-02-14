
The board is programmed through the serial port pins with an FTDI cable. Not through the USB port
    There is a FTDI header on the shield

To test with vkeybd:
    vkeybd --addr 24:0
    The address is the combination of client:port as per /proc/asound/seq/clients
    Testing sysex: amidi -p hw:2,0,0 -S "F0 00 00 <d1> <d2> <d3> F7"


software
	sysex:
		Can be tested with: amidi -p hw:2,0,0 -S "F0 00 00 <d1> <d2> <d3> F7"
		Set channel to listen to for pitch CV 1
			d1 = 0x10
			d2 = CV number (0..2)
			d3 = channel (0..16)
		Set channel to listen to for CC CV 1
			d1 = 0x20
			d2 = CC number (0..2)
			d3 = channel (0..16)
		Set CC number to listen to for CC CV 1
			d1 = 0x30
			d2 = CC number (0..2)
			d3 = CC number (0..255)
		Set output clock pulse width 
			d1 = 0x40
			d2 = width in ms. 5..255
	 	Write config to EEPROM	
			d1 = 0x55
			d2 = 0x55
			d3 = 0x55
		


hardware
    panel-mount din5
    ftdi programmer
