# GB2Serial

A simple stepping stone projects that reads from the Gameboy Link cable using software SPI and pass it to the hardware UART (hardware serial).
This is a test (but standalone part) of the soft SPI library.

## Why not hardware SPI?
Because it's used for for the ISP programmer, and both programmer & GB talking at the same time will confuse the mCU or forbid me to program it with my current setup. And I like/need a (small) challenge.
