#pragma once

#include<cstdint>
#include "MEMORY.h"
#include "Graphics.h"

class CPU
{
	private:
		unsigned short opcode;
		unsigned char* V;
		unsigned short I;

		//Program counter points at opcode
		// to execute
		unsigned short pc;

		unsigned short stack[16];
		unsigned short sp;

		unsigned char* keys;


		void fetch();
		void decode();
		void execute();
		
		Memory* memory;
		Graphics* graphics;
		
		// Implement something to 
		// Update timers.
		

		//Implementation of opcodes
		// https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description
		void _0NNN();
		void _00E0();
		void _00EE();
		void _1NNN();
		void _2NNN();
		void _3XNN();
		void _4XNN();
		void _5XY0();
		void _6XNN();
		void _7XNN();
		void _8XY0();
		void _8XY1();
		void _8XY2();
		void _8XY3();
		void _8XY4();
		void _8XY5();
		void _8XY6();
		void _8XY7();
		void _8XYE();
		void _9XY0();
		void _ANNN();
		void _BNNN();
		void _CXNN();
		void _DXYN();
		void _EX9E();
		void _EXA1();
		void _FX07();
		void _FX0A();
		void _FX15();
		void _FX18();
		void _FX1E();
		void _FX29();
		void _FX33();
		void _FX55();
		void _FX65();
		
		typedef void (CPU::*Chip8Table)();

		Chip8Table* defaultTable;
		Chip8Table* zerosTable;
		Chip8Table* eightsTable;
		Chip8Table* EsTable;
		Chip8Table* FsTable;

		// Functions to call tabels
		void _TableZeros();
		void _TableEights();
		void _TableEs();
		void _TableFs();

		void initalizeLookupTables();

	public:
		void setMemory(Memory* memory);
		void setGraphics(Graphics* graphics);
		void setKeys(unsigned char* keys);
		void emulateCycle();
		void initialize();

		unsigned char delay_timer;
		unsigned char sound_timer;

		void cpuNULL();
};
