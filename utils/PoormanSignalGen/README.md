# Poor man Signal Generator

Because I don't have one and *sometimes* you need to get a know signal to test stuffs ... like when you make software SPI on a device like a GameBoy.

## Signal

2 wires CLK + MOSI generation:
1. CLK: new data available on rising edge
1. MOSI: sends 0xCA (b11001010) which is very nice to detect