# Pico-SD---IDE-interface---for-Rc2014

A quick guide to get it working

The Hardware can be purchaced from Z80 kits (RC2014) https://z80kits.com/shop/micro-sd-card-module/

### SD card
most modern SD cards appear to work. Ideally type 10 or faster
The card size needs to be >512Mb
Format the card to FAT32 this appears to be the default for most new cards anyway. 

Copy the contents of the SD Card directory (unzipping any compressed files) onto your SD card

You need 
An img file of the CPM image with either SIO or ACIA drivers
CPM Inc Transient Apps ACIA.img or CPM Inc Transient Apps SIO2.img (the CPM image)

A CPMIDE.id file, a diskdefs file and and rc2014.ini file.

The rc2014.ini file will need to be edited depending which serial img you use there are two lines
#idefile = "CPM Inc Transient Apps ACIA.img";
#idefile = "CPM Inc Transient Apps SIO2.img";
remove the hash for the one that matches your serial hardware and save the ini file back to the SD card.

Plug in the SD card into the module and the SD card modure into your RC2014 Card
If you have an extened bus, that should be all you need. 

If you don't have an extended bus, you will need to remove the wait jumper on your processor card and
run a wire from the Z80 wait signal line to the wait pin on the SD module.

and that is it. 

### Issues ###

Using download.com for file transfer will require a much longer serial delay due to pauses when writing to disk. See the Fast File Transfer below, for a faster solution.

### Fast File Transfer

Connect via the USB port and transfer programs directly into CPM from a Windows or Linux machine _MUCH_ faster then using command.com
Cleck out the fast file transfer programs in  SerialCPMtools.md 

## RC2040
The SD images are directly compatable with the RC2040 https://github.com/ExtremeElectronics/RC2040 an RC2040.ini will work in both an RC2040 and an RC2014. The RC2014.ini will only work in an RC2014 as it only has a subset of the settings needed. The RC2040 will of course require the ROM binarys too and these will be ignored by the RC2014.

## Copying from an existing CF card. 

# CURRENTLY BEING TESTED USE AT YOUR OWN PERIL

A complete CPM system can be moved from a CF card to an SD card. In Linux, The process is as follows. 

**Be very carefull as dd can trash your system if you get this wrong

Create a dump of the CF card with dd 

/dev/cfdrive is the path to your physical plugged in CF card, change this to suit your own system

/home/cfimage.img will be the file image of your CF card, change this to save onto your own system

/home/sdfile.img will be the CPM file system image to be saved to your SD card (change RC2014.ini to match), change this to save onto your own system

dd if=/dev/cfdrive of=/home/cfimage.img

dd if=/home/cfimage.img of=/home/CPMIDE.id bs=1024 count=1

dd if=/home/cfimage.img of=/home/sdfile.img bs=1024 skip=1

# SPO256AL2
Using the same card, without the SD card (or socket) you can emulate a SPO256-al2 and give your RC2014 the ability to play music
https://github.com/ExtremeElectronics/Pico-SP0256-AL2-Em-for-Rc2014-SD-CARD-Module


