
#offset to ident information
ios = 512


def PrintIntegerIdent(s, b):
    v = ident[b+ios]+256*ident[b+1+ios]
    print(hex(b+ios)," ", end='')
    print(s+":", v)


def PrintMagic():
    print("Magic: ", end='')
    for i in range(8):
        print(chr(ident[i]), end='')
    print('')


def PrintCharIdent(s, b, l):
    print(hex(b + ios)," ", end='')
    print(s+":", end='')
    for i in range(b, l+b):
       print(chr(ident[i+ios]), end='')
    print('')


#f = open('CPMIDE.id', 'rb')
f = open('ROMWBW.id', 'rb')
ident = bytearray(f.read())

PrintMagic()

PrintIntegerIdent("Config/ID", 0)
PrintIntegerIdent("Fixed Cylinders", 2)
PrintIntegerIdent("Rem Cylinders", 4)
PrintIntegerIdent("Heads", 6)
PrintIntegerIdent("unf bytes per phy track", 8)
PrintIntegerIdent("unf bytes per sector", 10)
PrintIntegerIdent("Sectors per tracks", 12)
PrintIntegerIdent("Bytes in inter sector gap", 14)
PrintIntegerIdent("bytes in sync fields", 16)
#PrintIntegerIdent("0", 18)
PrintCharIdent("Serial Number", 20, 20)
PrintIntegerIdent("Cont Type", 40)
PrintIntegerIdent("Cont Buff Size", 42)
PrintIntegerIdent("No of ECC bytes on long cmds", 44)
PrintCharIdent("Controller Firmware ver", 46, 8)
PrintCharIdent("ModelNumber", 54, 40)
PrintIntegerIdent("Current No Cylindars", 108)
PrintIntegerIdent("Current No heads", 110)
PrintIntegerIdent("Current Sectors per Track", 112)
PrintIntegerIdent("Current capacity in Sectors", 114)

PrintIntegerIdent("Current accessable Sectors", 120)


