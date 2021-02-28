#include <iostream>

#include "Chip8.h"
#include "SDL.h"

// TODO: Implement SDL and start to work on graphical output


int main(int argc, char** argv)
{

	Chip8 Chip;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("ChipEmu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,64,32, NULL);
	
	
	bool isRunning = true;

	Chip.LoadROM();

	while (isRunning) {
		Chip.EmulateCycle();
	}

	return 0;
}