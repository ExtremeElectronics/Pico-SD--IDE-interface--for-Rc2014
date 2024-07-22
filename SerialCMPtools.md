# Experimental Fast file transfer 

### This is _very_ experimental at the moment

Strongly suggest flashing the latest (not released) software if you are trying this.

It uses the USB for file transfer direct to the CPM file system

The python programs here https://github.com/ExtremeElectronics/RC20XX-file-transfer-programs should run with most installs of python 3
on Windows or Linux

They allow the basic file commands CopyTo, CopyFrom, RM, CAT and LS to work over the Serial USB connection direct to the CPM file system on your SD card at around 15Kb/s (0.16 Z/s)* should be doable for CopyTo and 52Kb/s (0.53 Z/s)* for CopyFrom

CPM doesn't know files have been dumped into a dirive externally, so you may have to move into a different drive (letter) and back again if you are in the drive you are dumping files to, to see the changes. 

More details can be found in the RC20XX-file-transfer-programs Git hub https://github.com/ExtremeElectronics/RC20XX-file-transfer-programs 

If you wish to use the serial uart insted of the USB connection, you will need to re-compile and change the CMakeLists.txt  stdio with the lines

pico_enable_stdio_usb(z80disk  0)
pico_enable_stdio_uart(z80disk  1)

 ** Zorks per second where 1Zork=96K (aparenty, this is the standard measure of file transfer speed in an RC2014)

