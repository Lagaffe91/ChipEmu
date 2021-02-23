# ChipEmu
 My personal Chip8 emulator that i made to teach myself how a basic emulator work

## Progress
Here is the current progress of the project :

- [ ] Chip Emulation
  - [x] Memory Emulation
  - [x] Registers Emulation
  - [x] Fontset 
  - [x] Reset Methods (Memory,	Display, Registers, Stack)
  - [x] Load ROM into memory
  - [ ] Interpreter : Basic Emulation loop
  - [ ] Interpreter : Opcodes implementation (20/34)
 
- [ ] Graphical output
  - [ ] Inputs handeling
  - [ ] Sound
  - [ ] Image Rendering
  - [ ] User Input

## Building

First, you need [CMake 3.19](https://cmake.org/) or newer.

Then build the emulator with the following commands :

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```