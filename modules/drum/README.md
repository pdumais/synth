# Building
source the esp environment, in my case it's located under ~/projects/esp32, so:
    . ~/projects/esp32/esp-idf/export.sh

To build: idf.py build
To build and flash partition table: Only need to do this once
    idf.py partition_table
To upload drumkit samples: 
    (cd drumkit && tar -cf ../data.tar *)
    esptool.py -p /dev/ttyUSB9 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x20000 data.tar
To install: idf.py app-flash -p /dev/ttyUSB9
Monitor output for debugging: idf.py monitor -p /dev/ttyUSB9
