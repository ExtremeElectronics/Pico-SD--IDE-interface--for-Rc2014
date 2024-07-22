# SD Card Contents for ROMWBW

Format your SD card as FAT 32

For *Newer versions of ROMWBW use the RomWbW-RC2014-Zed-SD-2024.zip extract all the files to the root of your SD card, this contains all the files you need.

### *Older ROM WBW SD card disk image

For Newer versions of ROMWBW use the ROMWBW OLDer.zip

Extract all the files to the root of your SD card, this contains all the files you need.

### Notes
Zdos sees drives A:-J:
Running CPM directly you will only get A:-C:

### Using FFS
The disks will appear as A: - H: regardless of the mapping of RomWBW 
Only drives A: to H: will be accessible, currently, trying to get to drives I: - P: will crash the SD adapter, you have been warned!
Strongly suggest using the latest (not released) software if you are trying this.

### two IDE drives. 
if you flash the pico with the latest (not released) software you can add 
idefile1 = "another.img" to the RC2014 config and see both IDE drives.

### *Older and newer 
At some point ROMWBW changed the way they read IDE disks, sorry I dont know at what version this happened. Some time before RomWBW HBIOS v3.4.0  2023-12-22?
