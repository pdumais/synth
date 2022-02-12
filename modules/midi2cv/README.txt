The board is programmed through the serial port pins with an FTDI cable. Not through the USB port

To test with vkeybd:
    vkeybd --addr 24:0
    The address is the combination of client:port as per /proc/asound/seq/clients
