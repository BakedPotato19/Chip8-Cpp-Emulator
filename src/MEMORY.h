#pragma once

class Memory
{
	private:
		unsigned char map[4096];

	public:
		unsigned char readMemory(int address) ;
		void setMemory(int address, unsigned char value) ;
		
};
