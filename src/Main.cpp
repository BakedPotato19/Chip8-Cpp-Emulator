#include "CHIP8.h"
#include<iostream>

int main(int argc, char* argv[]){
	CHIP8* chip = new CHIP8();
	chip->initialize();
	if(chip->loadRomToMemory(argv[1]))
	{
		chip->loop();
	}

	delete chip;
	return 0;
	
}
