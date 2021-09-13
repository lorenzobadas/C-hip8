# C-hip8
C-hip8 is a Chip-8 emulator written in C, graphical interface is made with SDL2.
## Build project
To build the project go to the root directory and enter these commands:
```
mkdir build
cd build
cmake ..
make
```
## Usage
```
./chip8_emulator <ROM path> <Delay> <Scale>
```
Suggested setting for <Delay> is 2000.  
Suggested setting for <Scale> is 20.

Launching the program without commandline arguments will just display a Chip-8 logo on a new window.
