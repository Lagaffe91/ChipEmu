#include <string>
#include <fstream>
#include <Windows.h>
#include <iostream>

#include "Chip8.h"


Chip8::Chip8() {
	this->Reset();
	std::printf("CHIP-8 : Emulated chip succesfully created");
}


void Chip8::Reset() 
{
	this->pc	 = 0x200;
	this->opcode = 0 ;
	this->sp	 = 0 ;
	this->I		 = 0 ;
	
	this->DelayTimer = 0;
	this->SoundTimer = 0;

	
	this->ResetDisplay();	//Work
	
	this->ResetStack();		//To test
	
	this->ResetRegisters();	//To test

	this->ResetMemory();	//Work
	
	this->LoadFontSet();	//Work

	printf("CHIP - Succesfull Reset\n");
}


void Chip8::LoadROM()
{
	std::ifstream input = Chip8::SelectFile();
	if (!input.is_open())
		printf("CHIP-8 : Error while loading the ROM");
		return;
	this->ResetMemory();
	this->LoadFontSet();
	for (int i = 0; !input.eof(); i++) 
	{
		this->memory[i+512] = input.get();
	}
	input.close();
}


void Chip8::EmulateCycle()
{
	this->FetchOpcode();
}


std::ifstream Chip8::SelectFile() {
	
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = { 0 }; //Tchar ? May change it

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Chip8 (*.ch8)\0*.ch8\0All (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	std::ifstream input;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		input = std::ifstream(ofn.lpstrFile, std::ios::binary);
	}
	else {
		std::cout << "Error while opening the file (Aborted by user)\n";
	}

	if (input.is_open())
	{	
		return input;
	}
}


void Chip8::ResetRegisters() 
{
	for(int i = 0 ; i < 16 ; i++)
	{
		this->V[i] = 0 ;
	}
}


void Chip8::ResetDisplay() 
{
	for (int i = 0; i < (32 * 64)-1; i++) {
		this->Display[i] = 0;
	}
	std::printf("CHIP - Display was reset");
}


void Chip8::ResetStack()
{
	for (int i = 0; i < 16; i++)
	{
		this->stack[i] = 0;
		std::printf("Stack %i : %#x\n", i, this->stack[i]);
	}
	this->sp = 0;
}


void Chip8::ResetMemory() {
	for (int i = 0; i < MEMORY_SIZE; i++) {
		this->memory[i] = 0; 
	}
}


void Chip8::LoadFontSet() {
	
	for (int i = 0; i < FONTSET_SIZE ; i++) {
		this->memory[i] = this->chip8_fontset[i];
		std::printf("CHIP - Fontset %i : %#x \n",i,this->memory[i]);
	}
}


void Chip8::FetchOpcode() 
{
	this->opcode = memory[pc] << 8 | memory[pc + 1];
	std::printf("%#x", this->opcode);
}


/*

MEMORY :

(0x000-0x200)512 first bytes -> font data (?)
program start at location 512 (0x200)
(0xF00-0xFFF) display refresh
(0xEA0-0xEFF) call stack

*/