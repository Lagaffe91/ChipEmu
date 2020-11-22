#include <iostream>

#include "Chip8.h"
#include "Emulator.h"

int main(int argc, char** argv)
{
	Chip8 Chip;

	Emulator::InitEmulator();

	Chip.Reset();
}