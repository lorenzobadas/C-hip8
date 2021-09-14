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
./c-hip8 <ROM path> [-d <Delay>] [-s <Scale>]
```
Default setting for <Delay> is 2000.  
Default setting for <Scale> is 20.

Launching the program without any commandline arguments will just display a Chip-8 logo on a new window.
