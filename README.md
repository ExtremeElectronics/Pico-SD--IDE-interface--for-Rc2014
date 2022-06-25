# Pico-SD---IDE-interface---for-Rc2014

A quick guide to get it working

SD card, most modern SD cards appear to work.Ideally Type 10 and faster
The size needs to be >1G 
Format the card to FAT32 which appres to be the default for most new cards anyway. 

Copy the contents of the SD Card directory (unzipping any compressed files) 

You need 
an img file of the CPM image with either of SIO or ACIA drivers
CPM Inc Transient Apps ACIA.img or CPM Inc Transient Apps SIO2.img
a CPMIDE.id file a diskdefs file and and rc2014.ini file
The rc2014 file will need to be edited depending which serial img you use there are two lines
#idefile = "CPM Inc Transient Apps ACIA.img";
#idefile = "CPM Inc Transient Apps SIO2.img";
remove the hash for the one that matches your serial terminal and save the ini file back to the SD card.

plug in the SD card into the module and the sd card modure into your RC2014
If you ahve an extened bus that shoudl be all you need. 

If you don't have an extended bus, you will need to remove the wait jumper on your processor card and
run a wire from the wait signal line to the wait pin on the SD module.

and that is it. 

Also
Cleck out the fast file transfer programs in the SerialCPMtools.md
