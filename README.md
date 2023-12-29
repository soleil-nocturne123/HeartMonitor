# HeartMonitor
Portable Photoelectric Heart Monitor with EFM8 Board (8051 family). This is a documented version of a mini project done by Chantel and Hoai Huong (Sylvia) in ELEC 292 (Feb 2023) with some brush-ups on the additional features unfinished by the time we did the demonstration. Please refer to HeartMonitor.pdf for detailed information on the design.

## Comments
Back when we did this project, we used CrossIDE written by Dr. Jesús Calviño-Fraga to automatically compile, link and load our code. This is very convenient but at the same time, also mask us from truly understand the process from compilation, linkage to the destination file that can be uploaded to the microcontroller target. Because of that, in this newer version, we decided to use CALL51 to expose ourself more to a 8051 toolchain.  

## Resources we referred to
[1] ELEC292/291 (UBC) Course Notes by Dr. Jesús Calviño-Fraga 

### Firmware
This directory contains our code:\
**setup.c** -- Code to set up the microcontroller\
**LCD.c** -- Code with support functions for LCD display\
**TimerCounter** -- Code with support functions in interaction with Timers and Counters\
**main.c** -- Main code with execution

### Hardware
This directory contains the simulation of our hardware.
