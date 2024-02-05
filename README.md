# GameboyClock
Code for a Gameboy as a clock project: the idea is to use your gameboy to display the time. Absolutely useless, yes. But so much fun to do!

All the details on the main site: http://electrogeek.cc/gameboy%20clock.html#SerialMode

## Approach
Here I use the approach of having a device on the Link port to emulate another GB, that will answer the time when asked by the main (= display) GB.

# Contents

## Utils/
Some utilitary programs such as:
- GB2Serial : an "echo to serial" what is received from the GB to the UART of the mUC. Implements the SoftSPI I wrote for the occasion. 
- PoorManSignalGen : just a poor man signal generator for the tests since I don't have one. 
![oscillo signal](https://github.com/AlanFromJapan/GameboyClock/blob/main/utils/PoormanSignalGen/Resources/20240203_172842.jpg?raw=true)
