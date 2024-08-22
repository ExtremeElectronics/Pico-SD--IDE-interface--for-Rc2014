# The ID File

An incomplete description of the ID file for the RC2040 RC2014 SD card intterface and the RC2040P2 emulation of ROMWBW

The ID file can be the first 1024 bytes of a CF image or more common, it is split out to a seprate file so CPMTools can be used on the disk information without an offset.

The ID file is 1024 bytes of information. This is split into two sections.

### Section 1
The first 512 bytes contains a magic number 1DED15C0 this proves the ID file is correct for the system used.

### Section 2

This follows the structure of the CPM IDENTIFY DRIVE command 0xED

The structure is below, but the important items for changing the disk geometry are Default CHS (words 0x202 0x204 0x206) and Current CHS (0x26c, 0x26e, 0x270) 

and less so, LBAcapacity (words 60-61) and 48-bit capacity (words 100-103).

```
Offset 		Description 							Example
0x200		bit field: bit 6: fixed disk, bit 7: removable medium 	0x0040
0x202		Default number of cylinders 			16383
0x204		Default number of heads 				  16
0x206		Default number of sectors per track 	63
0x210		bytes in sync field						    0
0x214		Serial number (in ASCII) 				  G8067TME
0x22e		Firmware revision (in ASCII) 			GAK&1B0
0x231		bit field: bit 9: LBA supported 	0x2f00
0x235		bit field: bit 0: words 54-58 are valid 0x0007
0x236		Model name (in ASCII) 					  Maxtor 4G160J8
0x26c		Current number of cylinders 			16383
0x26e		Current number of heads 				  16
0x270		Current number of sectors per track 	63
0x272 		Current LBA capacity 					  16514064
0x278 		Default LBA capacity sectors		268435455

```
In the ASCII strings each word contains two characters, the high order byte the first, the low order byte the second. 

The 32-bit values are given with low order word first. 
