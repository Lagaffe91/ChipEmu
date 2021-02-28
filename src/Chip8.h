#pragma once
#include <string>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h> 

#define MEMORY_SIZE (4096)
#define FONTSET_SIZE (80)

class Chip8
{
	private:

		unsigned char memory[MEMORY_SIZE];
		unsigned char V[16];

		unsigned int DelayTimer;
		unsigned int SoundTimer;

		unsigned short opcode;

		unsigned char Display[64][32];

		unsigned short pc = 0x200;		//Program Start at 0x200
		unsigned short I;

		unsigned short stack[16];
		unsigned short sp;

		unsigned short KeyPad[16];

		unsigned char chip8_fontset[80] =
		{
		  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		  0x20, 0x60, 0x20, 0x20, 0x70, // 1
		  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
		};

		void ResetMemory() //Work
		{
			for (int i = 0; i < MEMORY_SIZE; i++) {
				this->memory[i] = 0;
			}
		}

		void ResetDisplay() //To test
		{
			for (int k = 0; k < 64; k++)
			{
				for (int i = 0; i < 32; i++)
				{
					this->Display[i][k] = 0;
				}
			}
			std::printf("\nCHIP - Display was reset\n");
		}

		void ResetRegisters()	//To test
		{
			for (int i = 0; i < 16; i++)
			{
				this->V[i] = 0;
			}
		}

		void ResetStack() //To test
		{
			for (int i = 0; i < 16; i++)
			{
				this->stack[i] = 0;
			}
			this->sp = 0;
		}

		void LoadFontSet() //Work
		{
			for (int i = 0; i < FONTSET_SIZE; i++) {
				this->memory[i] = this->chip8_fontset[i];
			}
		}


		void FetchOpcode()
		{
			this->opcode = memory[pc] << 8 | memory[pc + 1];
			std::printf("CHIP - Fetch opcode : %#x \n", this->opcode);
		}

		void ExecuteOpcode()
		{
			std::printf("CHIP - Execute opcode : %#x \n", this->opcode);

			switch (this->opcode & 0xF000)
			{

			case 0x0000:
			{
				//00E0 - CLS : Clear the display
				if ((this->opcode & 0xF00F) == 0x00E0)
				{
					this->ResetDisplay();
					break;
				}

				//00EE - RET
				if ((this->opcode & 0xF00F) == 0x00EE)
				{
					this->pc = this->stack[this->sp];
					this->sp--;
				}

				//0nnn - SYS addr - This opcode is ignored
				break;
			}

			//1nnn - JP addr
			case 0x1000:
			{
				this->pc = this->opcode & 0x0FFF;
				break;
			}
			//2nnn - CALL addr
			case 0x2000:
			{
				this->sp++;
				this->stack[this->sp] = this->pc;
				this->pc = this->opcode & 0x0FFF;
				break;
			}

			//3xkk - SE Vx, byte
			case 0x3000:
			{
				if (V[this->opcode & 0x0F00] == (this->opcode & 0x00FF))
				{
					this->pc += 2;
				}
				this->pc += 2;
				break;
			}

			//4xkk - SNE Vx, byte
			case 0x4000:
			{
				if (V[this->opcode & 0x0F00] != (this->opcode & 0x00FF))
				{
					this->pc += 2;
				}
				this->pc += 2;
				break;
			}

			//5xy0 - SE Vx, Vy
			case 0x5000:
			{
				if (this->V[this->opcode & 0x0F00] == this->V[this->opcode & 0x00F0])
				{
					this->pc += 2;
				}
				this->pc += 2;
				break;
			}

			//6xkk - LD Vx, byte
			case 0x6000:
			{
				this->V[this->opcode & 0x0F00] = this->opcode & 0x00FF;
				this->pc += 2;
				break;
			}

			//7xkk - ADD Vx, byte
			case 0x7000:
			{
				this->V[this->opcode & 0x0F00] += this->opcode & 0x00FF;
				this->pc += 2;
				break;
			}


			case 0x8000:
			{
				//8xy0 - LD Vx, Vy
				if ((this->opcode & 0xF00F) == 0x8000)
				{
					this->V[this->opcode & 0x0F00] = this->V[this->opcode & 0x00F0];
					this->pc += 2;
					break;
				}

				//8xy1 - OR Vx, Vy
				if ((this->opcode & 0xF00F) == 0x8001)
				{
					unsigned short buffer;
					buffer = this->V[this->opcode & 0x0F00] | this->V[this->opcode & 0x00F0];
					this->V[this->opcode & 0x0F00] = buffer;
					break;
				}

				//8xy2 - AND Vx, Vy
				if ((this->opcode & 0xF00F) == 0x8002)
				{
					unsigned short buffer;
					buffer = this->V[this->opcode & 0x0F00] & this->V[this->opcode & 0x00F0];
					this->V[this->opcode & 0x0F00] = buffer;
					break;
				}

				//8xy3 - XOR Vx, Vy
				if ((this->opcode & 0xF00F) == 0x8003)
				{
					unsigned short buffer;
					buffer = this->V[this->opcode & 0x0F00] ^ this->V[this->opcode & 0x00F0];
					this->V[this->opcode & 0x0F00] = buffer;
					break;
				}

				//8xy4 - ADD Vx, Vy
				if ((this->opcode & 0xF00F) == 0x8001)
				{
					unsigned short buffer;
					buffer = this->V[this->opcode & 0x0F00] + this->V[this->opcode & 0x00F0];
					if (buffer > 255)
					{
						this->V[0xF] = 1;
						buffer &= 0x00FF;
					}
					this->V[this->opcode & 0x0F00] = buffer;
					break;
				}

				//8xy5 - SUB Vx, Vy
				if ((this->opcode & 0xF00F) == 0x8005)
				{
					if (this->V[this->opcode & 0x0F00] > this->V[this->opcode & 0x00F0])
					{
						this->V[0xF] = 1;
					}
					else
					{
						this->V[0xF] = 0;
					}

					this->V[this->opcode & 0x0F00] -= this->V[this->opcode & 0x00F0];
					break;
				}

				//8xy6 - SHR Vx{ , Vy }
				if ((this->opcode & 0xF00F) == 0x8006)
				{
					if ((this->V[this->opcode & 0x0F00] & 1) == 1)
					{
						this->V[0xF] = 1;
					}
					else
					{
						this->V[0xF] = 0;
					}
					this->V[this->opcode & 0x0F00] = this->V[this->opcode & 0x0F00] / 2;
					break;
				}

				//8xy7 - SUBN Vx, Vy
				if ((this->opcode & 0xF00F) == 0x8007)
				{
					if (this->V[this->opcode & 0x00F0] > this->V[this->opcode & 0x0F00])
					{
						this->V[0xF] = 1;
					}
					this->V[this->opcode & 0x0F00] -= this->V[this->opcode & 0x00F0];
					break;
				}

				//8xyE - SHL Vx{ , Vy }
				//TODO: Implement 0x8xyE
				if ((this->opcode & 0xF00F) == 0x800E)
				{
					break;
				}

				break;
			}

			//9xy0 - SNE Vx, Vy
			case 0x9000:
			{
				if (this->V[this->opcode & 0x0F00] != this->V[this->opcode & 0x00F0])
				{
					this->pc += 2;
				}
				this->pc += 2;
				break;
			}

			//Annn - LD I, addr
			case 0xA000:
			{
				this->I = this->opcode & 0x0FFF;
				this->pc += 2;
				break;
			}

			//Bnnn - JP V0, addr
			case 0xB000:
			{
				this->pc = (this->opcode & 0x0FFF) + this->V[0];
				break;
			}

			//Cxkk - RND Vx, byte
			case 0xC000:
			{
				int random = rand() % 256;
				this->V[this->opcode & 0x0F00] = random & (this->opcode & 0x00FF);
				this->pc += 2;
				break;
			}

			//Dxyn - DRW Vx, Vy, nibble
			case 0xD000:
			{
				for (int i = 0; i < (this->opcode & 0x000F); i++)
				{

					//Xor things at location "this->memory[this->I + i]" in the memory to coordonate (Vx, Vy);
				}


				break;
			}
			//Ex9E - SKP Vx
			//ExA1 - SKNP Vx
			//Fx07 - LD Vx, DT
			//Fx0A - LD Vx, K
			//Fx15 - LD DT, Vx
			//Fx18 - LD ST, Vx
			//Fx1E - ADD I, Vx
			//Fx29 - LD F, Vx
			//Fx33 - LD B, Vx
			//Fx55 - LD[I], Vx
			//Fx65 - LD Vx, [I]
			//TODO: "{, Vy}" opcode to check, there may be something more to do (8xy6 - 8xyE)
			//TODO: Finish to implement all opcodes

			exit(0); //Invalid opcode, may change that later
			break;
			}
		}


		static std::ifstream SelectFile() {

			OPENFILENAME ofn;
			TCHAR szFile[MAX_PATH] = { 0 }; //Tchar ? May change it

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "Chip8 (*.ch8)\0*.ch8\0All (*.*)\0*.*\0";
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
				std::printf("ROM - Error while opening the file (Aborted by user)\n");
				exit(0); //I'd like to do somthing better than just crash the program
			}

			return input;
		}

	public :
		Chip8()
		{
			this->Reset();
			std::printf("CHIP-8 : Emulated chip succesfully created\n");
			srand(time(NULL));
		}


		/*Reset the chip at defaults values*/
		void Reset()
		{
			this->pc = 0x200;
			this->opcode = 0;
			this->sp = 0;
			this->I = 0;

			this->DelayTimer = 0;
			this->SoundTimer = 0;


			this->ResetDisplay();

			this->ResetStack();

			this->ResetRegisters();

			this->ResetMemory();

			printf("CHIP - Succesfull Reset\n");
		}
		
		/*Select a file/rom and load it into chip memory*/
		void LoadROM()
		{
			std::ifstream inputFile;
			inputFile = Chip8::SelectFile();
			if (!inputFile.is_open())
			{
				printf("CHIP - ! Error while loading the ROM !");
				return;
			}
			this->ResetMemory();
			this->LoadFontSet();
			for (int i = 0; !inputFile.eof(); i++)
			{
				this->memory[i + 512] = inputFile.get();
			}
			inputFile.close();
		}


		/*Fetch, Decode, And execute the next opcode*/
		void EmulateCycle()
		{
			this->FetchOpcode();
			this->ExecuteOpcode();

			if (this->DelayTimer > 0)
			{
				this->DelayTimer--;
			}

			if (this->SoundTimer > 0)
			{
				if (this->SoundTimer == 1)
				{
					printf("BEEP!\n");
					//TODO: Implement sound
				}
				this->SoundTimer--;
			}

		}
};

/*-------------------------NOTES:-------------------------
*
*MEMORY :
*
*(0x000-0x200)512 first bytes -> font data (?)
*program start at location 0x200 (512)
*(0xF00-0xFFF) display refresh
*(0xEA0-0xEFF) call stack
*
*
*INSTRUCTION SET :
*
*Chip8 instruction set : https://en.wikipedia.org/wiki/CHIP-8
*
*-------------------------------------------------------*/


