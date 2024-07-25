# Using RomWBW

The Pico-SD is fully compatible with RomWBW.  The standard RC2014 RomWBW ROM has
built-in support for an IDE interface and will automatically detect the Pico-SD.
All of the RomWBW operating systems and custom applications will work with the
Pico-SD.

In summary, all that is required to use the Pico-SD with RomWBW is to copy the
appropriate files onto the SD Card.

If you have any problems, you can get support by:

- Posting a message at the [RC2014 Google Group](https://groups.google.com/g/rc2014-z80)
- Submitting an issue at the [RomWBW GitHub Repository](https://github.com/wwarthen/RomWBW)
- Contacting Wayne Warthen at wwarthen@gmail.com

Please refer to the
[RomWBW User Guide](https://github.com/wwarthen/RomWBW/blob/master/Doc/RomWBW%20User%20Guide.pdf)
for complete documentation on RomWBW.

## Prerequisites

Your RCBus system must already have a RomWBW ROM installed.  The RomWBW
ROM itself is not part of the Pico SD.  Your system should be able to boot to the RomWBW
Boot Loader prompt prior to installed the Pico-SD module.

For an optimal experience, you should be using RomWBW v3.2 or greater.  See the "Legacy RomWBW"
section below if you wish to use the Pico-SD with a RomWBW version older than this.

## SD Card Preparation

To prepare your SD Card for use with RomWBW, download the RomWBW SD Card.zip file
from this directory.  Unzip the archive and copy all of the files to a FAT32 formatted
SD Card.  With the SD Card installed in your Pico-SD and the Pico-SD module installed
in your system, boot your RCBus system normally.

## Initial Test

At this point, the Pico-SD device should be detected by RomWBW.  Here is an example
of booting RomWBW with the Pico-SD installed.  Your boot display may show different
devices, but you should see the line starting with `IDE0:`.

```
RomWBW HBIOS v3.4.0, 2024-07-22

RCBus [RCZ80_std] Z80 @ 7.372MHz
0 MEM W/S, 1 I/O W/S, INT MODE 1, Z2 MMU
512KB ROM, 512KB RAM, HEAP=0x0F60, RTCDEF=0x20
ROM VERIFY: 00 00 00 00 PASS

SIO0: IO=0x80 SIO MODE=115200,8,N,1
SIO1: IO=0x82 SIO MODE=115200,8,N,1
DSRTC: MODE=STD IO=0xC0 Sun 2000-08-06 16:54:54 CHARGE=OFF
MD: UNITS=2 ROMDISK=384KB RAMDISK=256KB
IDE: IO=0x10 MODE=RC
IDE0: ATA 8-BIT LBA BLOCKS=0x000FC000 SIZE=504MB
IDE1: NO MEDIA
PPIDE: IO=0x20 PPI NOT PRESENT

Unit        Device      Type              Capacity/Mode
----------  ----------  ----------------  --------------------
Char 0      SIO0:       RS-232            115200,8,N,1
Char 1      SIO1:       RS-232            115200,8,N,1
Disk 0      MD0:        RAM Disk          256KB,LBA
Disk 1      MD1:        ROM Disk          384KB,LBA
Disk 2      IDE0:       Hard Disk         504MB,LBA
Disk 3      IDE1:       Hard Disk         --

RCBus [RCZ80_std] Boot Loader
FP Switches = 0x00

Boot [H=Help]
```

**IMPORTANT**: As you may know, it is important that RomWBW disk contents are
matched to the version of the RomWBW ROM that you are using.  The disk image
provided in the .zip file here are matched to RomWBW v3.4.x.  If you are using
a different version of the RomWBW ROM, you should follow the instructions
below to install the correct disk image on your SD Card.

## Updating the Disk Image

One of the files that you copied to your SD Card is called "hd1k_combo.img".
This file contains the contents of the Pico-SD disk drive as seen by RomWBW.
All releases of RomWBW starting with v3.2 will have this file in the Binary
directory of the distribution.  To install the disk image that matches your
ROM, you should get the hd1k_combo.img from the RomWBW Release that
matches your ROM and copy that onto your SD Card overwriting the previous
copy.

Advanced RomWBW users will frequently create their own custom
disk images.  Any valid RomWBW disk image can be used with the Pico-SD.
The combo image is described above because it is the most commonly used
disk image.

You can modify RC2014.ini file if you wish to change the name of the
disk image file that is used.  You can keep multiple
different disk image files on your SD Card and simply change the
`idefile` setting to refer to the disk image you want to use currently.
For example, if you want to periodically boot into UCSD p-System,
you can copy the psys.img from the RomWBW distribution to your SD
Card and then use `idefile = "psys.img";`


## Drive Letters

Keep in mind that RomWBW does not use fixed drive letters.  In order to
determine which drive letters refer to the Pico-SD, you must refer to
the boot messages.  You may also modify the drive letter assignments
using the RomWBW `ASSIGN` command.  If you are not familiar with this
RomWBW concept, it is worth reviewing the
[RomWBW User Guide](https://github.com/wwarthen/RomWBW/blob/master/Doc/RomWBW%20User%20Guide.pdf).

**WARNING**: Unlike a true hard disk, the size of the Pico SD disk is
limited by the size of the disk image file.  For example, the
hd1k_combo.img file contains 6 slices.  Attempting to refer to a
slice outside of the range 0-5 will cause unpredictable results
and may corrupt your disk.  You can create custom RomWBW disk
images with up to 256 slices to get around this if you like.

## Fast File Transfer

The FFS tool will refer to the drives on the SD Card as A: to H:.  These
are **not** the same as the RomWBW drive letters.  They are simply mapped
to the first 8 slices of the RomWBW disk image.  So, A: in FFS will refer
to the first slice, B: to the second slice, etc.

The FFS tools relies on the `diskdefs` file on the SD Card for the format
of the disk image.  The diskdefs supplied here are correct for the RomWBW
hd1k disk format.  They are not correct for the hd512 disk format.

Only drives A: to H: will be accessible, currently, trying to get to drives
I: to P: will crash the SD adapter, you have been warned!
Strongly suggest using the latest (not released) software if you are trying this.

## Dual IDE Drives

If you flash the Pico-SD  with the latest (not released) software you can add 
`idefile1 = "another.img";` to the RC2014 config and RomWBW will see both
drives.

## Legacy RomWBW

Versions of RomWBW older than v3.2 are considered legacy versions at this time.
You can use these older versions of RomWBW with the Pico-SD, but you must copy
a disk image from the corresponding RomWBW Release to your SD Card.  For the
legacy versions of RomWBW, the file you normally want is called hd_combo.img.
You must then update your RC2014.ini file to refer to this disk image.
