#include "CPU.h"
#include <bits/stdint-uintn.h>
#include <cstdio>
#include<iostream>
#include <ostream>
#include<stdlib.h>
#include<cstring>
#include<sstream>
#include<iomanip>

//=========================================================
// Non member functions : 
// definition of Lookup Tables
//=========================================================





//=========================================================
// Member functions of
// CPU class
//=========================================================


//=========================================================
// Call this right after instantiation of CPU
//=========================================================
void CPU::initialize()
{
	this->pc = 0x200; // Program counter 
	this->opcode = 0; // Opcode for next cpu instruction
	this->I = 0; 
	this->sp = 0; // Stack pointer
	this->V = new unsigned char[16];

	this->initalizeLookupTables();

	this->delay_timer = 0x00;
	this->sound_timer = 0x00;

	std::memset(V,0x0,sizeof(V));

}

//=========================================================
// Sets memory map of CPU. Standard is null, so this
// has to be set before usage!
//=========================================================
void CPU::setMemory(Memory* memory)
{
	this->memory = memory;
}

void CPU::setGraphics(Graphics* graphics)
{
	this->graphics = graphics;
}

void CPU::setKeys(unsigned char *keys)
{
	this->keys = keys;
}

void CPU::initalizeLookupTables()
{

	// Table converting opcode >> 12 to function calls	
	this->defaultTable = new Chip8Table[16]{
		&CPU::_TableZeros, &CPU::_1NNN, &CPU::_2NNN, &CPU::_3XNN, 
		&CPU::_4XNN, &CPU::_5XY0, &CPU::_6XNN, &CPU::_7XNN, 
		&CPU::_TableEights, &CPU::_9XY0, &CPU::_ANNN, &CPU::_BNNN, 
		&CPU::_CXNN, &CPU::_DXYN, &CPU::_TableEs, &CPU::_TableFs 
	};

	this->zerosTable = new Chip8Table[3]{
		&CPU::_0NNN, &CPU::_00E0, &CPU::_00EE
	};

	this->eightsTable = new Chip8Table[9]{
		&CPU::_8XY0, &CPU::_8XY1, &CPU::_8XY2, &CPU::_8XY3,
		&CPU::_8XY4, &CPU::_8XY5, &CPU::_8XY6, &CPU::_8XY7,
		&CPU::_8XYE
	};

	this->EsTable = new Chip8Table[2]{
		&CPU::_EXA1, &CPU::_EX9E
	};

}

//=========================================================
// Fetches, decodes and execute one CPU instruction.
//=========================================================
void CPU::emulateCycle()
{
	this->fetch();
	this->decode();
	this->execute();
}

//========================================================
// Fetches Memory at sp and updated opcode
//========================================================
void CPU::fetch()
{

	this->opcode = this->memory->readMemory(this->pc) << 8 
		| this->memory->readMemory(this->pc + 1);

	this->pc += 2;
}

void CPU::decode()
{
}

void CPU::execute()
{
	(this->*(this->defaultTable[(opcode&0xF000)>>12]))();

}

//========================================================
// This sections defines the 35 opcode functions
//========================================================

void CPU::_0NNN()
{
	// This is not used in modern CHIP8 implementations
}

void CPU::_00E0()
{
	std::memset(this->graphics->gfx, 0, sizeof(this->graphics->gfx));
}

void CPU::_00EE()
{
	this->pc = this->stack[--sp];
}

void CPU::_1NNN()
{
	this->pc = (this->opcode & 0x0FFF);
}


void CPU::_2NNN()
{
	stack[sp++]	= pc;
	pc = (opcode&0x0FFF);
}

void CPU::_3XNN()
{
	if(V[(opcode&0x0F00) >> 8] == (opcode&0x00FF))
		this->pc +=2;
}

void CPU::_4XNN()
{
	if(V[(opcode&0x0F00) >> 8] != (opcode&0x00FF))
		this->pc +=2;
}

void CPU::_5XY0()
{
	if(V[(opcode&0x0F00) >> 8] == V[(opcode&0x00F0) >> 4])
		this->pc +=2;
}

void CPU::_6XNN()
{
	// If bug happens - try to remove modulo
	V[(opcode&0x0F00) >> 8] = opcode&0x00FF;
}

void CPU::_7XNN()
{
	V[(opcode&0x0F00) >> 8] = 
		V[(opcode&0x0F00) >> 8] + (opcode&0x00FF);
}

void CPU::_8XY0()
{
	V[(opcode&0x0F00) >> 8] = V[(opcode&0x00F0) >> 4];
}

void CPU::_8XY1()
{
	V[(opcode&0x0F00) >> 8] = V[(opcode&0x00F0) >> 4] | V[(opcode&0x0F00) >> 8];
}

void CPU::_8XY2()
{
	V[(opcode&0x0F00) >> 8] = V[(opcode&0x00F0) >> 4] & V[(opcode&0x0F00) >> 8];
}

void CPU::_8XY3()
{
	V[(opcode&0x0F00) >> 8] = V[(opcode&0x00F0) >> 4] ^ V[(opcode&0x0F00) >> 8];
}

void CPU::_8XY4()
{
	V[15] = V[(opcode&0x00F0) >> 4] > (0xFF - V[(opcode&0x0F00) >> 8]);
	V[(opcode&0x0F00) >> 8]	+= V[(opcode&0x00F0) >> 4];
}

void CPU::_8XY5()
{
	V[15] = V[(opcode&0x0F00) >> 8] >= V[(opcode&0x00F0) >> 4];
	V[(opcode&0x0F00) >> 8] -= V[(opcode&0x00F0) >> 4];
}

void CPU::_8XY6()
{
	V[15] = V[(opcode&0x0F00) >> 8] & 0x0001;
	V[(opcode&0x0F00) >> 8] >>= 1;
}

void CPU::_8XY7()
{
	V[15] = V[(opcode&0x00F0) >> 4] > V[(opcode&0x0F00) >> 8];
	V[(opcode&0x0F00) >> 8] = V[(opcode&0x00F0) >> 4] - 
		V[(opcode&0x0F00) >> 8];
}

void CPU::_8XYE()
{
	V[15] = V[(opcode&0x0F00) >> 8] >> 7;
	V[(opcode&0x0F00) >> 8] <<=1;
}

void CPU::_9XY0()
{
	if(V[(opcode&0x0F00) >> 8] != V[(opcode&0x00F0) >> 4])
		this->pc +=2;
}

void CPU::_ANNN()
{
	this->I = (opcode&0x0FFF);
}

void CPU::_BNNN()
{
	this->pc = V[0] + (opcode&0x0FFF);
}

void CPU::_CXNN()
{
	V[(opcode&0x0F00) >> 8] = (rand()%0xFF)&(opcode&0x00FF);
}

void CPU::_DXYN()
{
	V[15] = 0;
	unsigned char xpos = V[(opcode&0x0F00) >> 8];
	unsigned char ypos = V[(opcode&0x00F0) >> 4];
	uint8_t n = (uint8_t)(opcode&0x000F);

	for(uint8_t y = 0; y < n; y++)
	{
		for(uint8_t x = 0; x<8;x++)
		{
			int gfx_pos = (xpos+x) % 64 + ((ypos+y) % 32)*64;

			if((memory->readMemory(I+y)&(0x0080>>x))!=0x0)
			{
				if(this->graphics->gfx[gfx_pos] == 0xFF)
				{
					V[15] = 1;
				}
				this->graphics->gfx[gfx_pos] ^=0xFF;
			}
		}
	}

}

void CPU::_EX9E()
{
	if(this->keys[V[(opcode&0x0F00) >> 8]] != 0)
		this->pc += 2;
}

void CPU::_EXA1()
{
	if(this->keys[V[(opcode&0x0F00) >> 8]] == 0)
		this->pc += 2;
}

void CPU::_FX07()
{
	V[(opcode&0x0F00) >> 8] = this->delay_timer;
}

void CPU::_FX0A()
{
	if(this->keys[0x0F]<0x10)
	{
		V[(opcode&0x0F00)>> 8] = keys[0x0F];
	}else{
		this->pc -= 2;
	}
}

void CPU::_FX15()
{
	this->delay_timer = V[(opcode&0x0F00) >> 8];
}

void CPU::_FX18()
{
	this->sound_timer = V[(opcode&0x0F00) >> 8];
}

void CPU::_FX1E()
{
	short l = this->I + V[(opcode&0x0F00) >> 8];
	V[15] = (l>0xFFF) ? 1 : 0;
	this->I = l;
}

void CPU::_FX29()
{
	this->I = (uint16_t) V[(opcode&0x0F00)>>8] * 5;
}

void CPU::_FX33()
{
	unsigned char vx = V[(opcode&0x0F00) >> 8];
	this->memory->setMemory(this->I,vx/100);
	this->memory->setMemory(this->I+1,(vx/10)%10);
	this->memory->setMemory(this->I+2,(vx%100)%10);
}

void CPU::_FX55()
{
	for(uint8_t i = 0; i<=((opcode&0x0F00) >> 8);i++ )
	{
		this->memory->setMemory(I+i,V[i]);
	}
}

void CPU::_FX65()
{
	for(uint8_t i = 0; i<=((opcode&0x0F00) >> 8);i++ )
	{
		V[i] = this->memory->readMemory(I+i);
	}
}

//=========================================================
// Lookup tables with function pointers
//=========================================================
// (this->*(this->defaultTable[8]))();
void CPU::_TableZeros()
{
	(this->*(this->zerosTable[(opcode&0x000F) > 0 ? 2 : 1]))();
}

void CPU::_TableEights()
{
	unsigned char index = (opcode&0x000F);
	(this->*(this->eightsTable[index < 8 ? index : 8]))();
}

void CPU::_TableEs()
{
	// Formats the 2x E opcodes
	(this->*(this->EsTable[(opcode&0x000F) > 1 ? 1 : 0]))();
}

void CPU::_TableFs()
{
	switch(opcode&0x00FF)
	{
		case 0x07:
			(this->*(&CPU::_FX07))();
			break;
		case 0x0A:
			(this->*(&CPU::_FX0A))();
			break;
		case 0x15:
			(this->*(&CPU::_FX15))();
			break;
		case 0x18:
			(this->*(&CPU::_FX18))();
			break;
		case 0x1E:
			(this->*(&CPU::_FX1E))();
			break;
		case 0x29:
			(this->*(&CPU::_FX29))();
			break;
		case 0x33:
			(this->*(&CPU::_FX33))();
			break;
		case 0x55:
			(this->*(&CPU::_FX55))();
			break;
		case 0x65:
			(this->*(&CPU::_FX65))();
			break;
	}
}


