# Experimental Fast file transfer 

### This is _very_ experimental at the moment

Currently this is only supported with the RC2014 SD module 

It uses the USB for file transfer direct to the CPM file system

The python programs here https://github.com/ExtremeElectronics/RC20XX-file-transfer-programs should run with most installs of python 3
(Currently Linux is untested, but I can't see why it shouldn't work) 

They allow the basic file commands CopyTo, CopyFrom, RM, CAT and LS to work over the Serial USB connection direct to the CPM file system on your SD card at speeds of up to 6K/

CPM doesn't know files have been dumped into a dirive externally, so you may have to move into a different drive and back again if you are in the drive you ae dumping files to, to see the changes. 

More details can be found in the RC20XX-file-transfer-programs Git hub https://github.com/ExtremeElectronics/RC20XX-file-transfer-programs 
