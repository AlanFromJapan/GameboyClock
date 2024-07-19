A POC for my GBLinkMini board with an ATtiny2313 and a DS1307.

# Point of interest

## GameBoy Link protocol
Using the same code I used in these other projects of that repo, hopefully with no change of the code and only the config file changed.

## Bit banging TWI on ATtiny2313

THAT will be the real interesting point. There's no onboard TWI (I2C) subsystem on the ATtiny2313, so it will have to be bitbanged.

The awesome thing is that ATmel (now Microchip) published code and paper how to do that! Just takes a bit of search on the internet. You want the **AVR156** paper and zip file.

- Paper https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ApplicationNotes/ApplicationNotes/doc42010.pdf
- Zip (forgot, you have to search, saved a copy in this repo anyway)
