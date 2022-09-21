# Pico-SD---IDE-interface---for-Rc2014

A quick guide to get it working

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

