#include <iostream>

#include "Chip8.h"

// TODO: Implement SDL and start to work on graphical output


int main(int argc, char** argv)
{
	Chip8 Chip;
	
	bool isRunning = true;

	Chip.LoadROM();

	while (isRunning) {
		Chip.EmulateCycle();
	}

	return 0;
}