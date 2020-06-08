#include "MEMORY.h"

unsigned char Memory::readMemory(int address)
{
	return this->map[address];
}

void Memory::setMemory(int address, unsigned char value)
{
	this->map[address] = value;
}



