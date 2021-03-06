#include <iostream>

#include "Chip8.h"
#include "sdl/sdl.h"

// TODO: Implement SDL and start to work on graphical output


int main(int argc, char** argv)
{

	Chip8 Chip;

	Chip.LoadROM();

	StartEmulation(Chip);

	return 0;
	
}