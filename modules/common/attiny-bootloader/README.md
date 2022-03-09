Optiboot Bootloader
This core includes an Optiboot bootloader for the ATtiny84/44, operating using software serial at 19200 baud - the software serial uses the AIN0 and AIN1 pins (see UART section below). The bootloader uses 640b of space, leaving 3456 or7552b available for user code. In order to work on the 84, which does not have hardware bootloader support (hence no BOOTRST functionality), "Virtual Boot" is used. This works around this limitation by rewriting the vector table of the sketch as it's uploaded - the reset vector gets pointed at the start of the bootloader, while the EE_RDY vector gets pointed to the start of the application.

Programming the ATtiny84/44 via ISP without the bootloader is fully supported; the 24 is supported only for ISP programming


Wiring for uploading with optiboot
    Gnd of serial adapter to Gnd of target
    Vcc of serial adapter to Gnd of target (many adapters have a switch to select the voltage)
    Tx of serial adapter to Rx of target
    Rx of serial adapter to Tx of target
    DTR of serial adapter to DTR of target, or if using self-build board, the other side of the 0.1uF autoreset capacitor

Wiring for ISP
    Vcc of programmer to Vcc of target
    Gnd of programmer to Gnd of target
    SCK of programmer (pin 13 on Uno/Nano/ProMini) to SCK of target
    MISO of programmer (pin 12 on Uno/Nano/ProMini) to MISO of target
    MOSI of programmer (pin 11 on Uno/Nano/ProMini) to MOSI of target



avrdude  -c stk500 -v -v -pt44 -P /dev/ttyUSB10 -B8 -U lfuse:w:0xE2:m -U hfuse:w:0xDC:m -U efuse:w:0xFE:m
avrdude  -c stk500 -v -v -pt44 -P /dev/ttyUSB10 -B8 -U flash:w:optiboot_attiny44_8000000L.hex:i
