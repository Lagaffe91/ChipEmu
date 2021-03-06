#pragma once

#include "../Chip8.h"

//This is ugly, but it's to avoid conflic with this sdl.h
#include "../include/SDL.h"

void StartEmulation(Chip8 Chip)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("ChipEmu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,720,480,NULL);

	/*bool isRunning = true;

	while (isRunning) {
		Chip.EmulateCycle();
	}*/
}