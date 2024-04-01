# RTCtester

Tests the RTC on the board and output time to UART (COM Serial).

## Fuses
I'm having ATmega328P**B** board which then needs the following fuses set like this by default (external 8MHz+ crystal):

    avrdude -pm328pb -cavrispmkII -U lfuse:w:0xee:m -U hfuse:w:0xd9:m
    
Since the time setting flag is saved in EEPROM, you might want to preserve that between programming cycles so set fuses like this (sets the *Preserve EEPROM memory through the Chip Erase cycle; [EESAVE=0]* in the HIGH fuse):

    avrdude -pm328pb -cavrispmkII -U lfuse:w:0xee:m -U hfuse:w:0xd1:m
