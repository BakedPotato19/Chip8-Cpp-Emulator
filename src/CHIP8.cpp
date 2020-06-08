#include "CHIP8.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include<iostream>
#include<bitset>
#include <ostream>

void CHIP8::initialize()
{
	this->chip8_fontset = new unsigned char[80]
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

	this->memory = new Memory();
	this->graphics = new Graphics();

	this->cpu = new CPU();
	this->cpu->initialize();
	this->cpu->setMemory(this->memory);
	this->cpu->setGraphics(this->graphics);
	this->cpu->setKeys(this->keys);

	this->loadFontsetToMemory();
	this->loadRomToMemory("/home/oliverbak/Downloads/Tetris [Fran Dachille, 1991].ch8");


	this->setupGraphics();
	this->loop();
	
}

void CHIP8::setupGraphics()
{
	this->window = new sf::RenderWindow(sf::VideoMode(640,320),"Chip8 emulator");
	this->window->setFramerateLimit(120);
	this->window->setVerticalSyncEnabled(false);
	this->dtClock = new sf::Clock();
}

void CHIP8::setupInput()
{

}

void CHIP8::loadFontsetToMemory()
{
	for (int i = 0; i<80; i++)	
	{
		this->memory->setMemory(i, this->chip8_fontset[i]);
	}
}

void CHIP8::loadRomToMemory(std::string path)
{
	std::ifstream rom;
    rom.open(path, std::ifstream::binary);
    
    if (rom) {
		std::cout << "rom exists" << std::endl;
        rom.seekg(0, rom.end);
        int buffer_size = rom.tellg();
        rom.seekg(0, rom.beg);

        char* buffer = new char [buffer_size];

        rom.read(buffer, buffer_size);
        rom.close();

        for (int i=0; i < buffer_size; i++) {
			this->memory->setMemory(i+0x200, buffer[i]);
        }

        delete[] buffer;
    }


}

void CHIP8::updateSFMLEvents()
{
	while(this->window->pollEvent(this->event))
	{
		if(this->event.type == sf::Event::Closed)
		{
			this->window->close();
		}
	

		if(this->event.type == sf::Event::KeyPressed)
		{
			switch(event.key.code)
			{
				case(sf::Keyboard::X):
					this->keys[0] = 0x1;
					break;
				case(sf::Keyboard::Num1):
					this->keys[1] = 0x1;
					break;
				case(sf::Keyboard::Num2):
					this->keys[2] = 0x1;
					break;
				case(sf::Keyboard::Num3):
					this->keys[3] = 0x1;
					break;
				case(sf::Keyboard::Q):
					this->keys[4] = 0x1;
					break;
				case(sf::Keyboard::W):
					this->keys[5] = 0x1;
					break;
				case(sf::Keyboard::E):
					this->keys[6] = 0x1;
					break;
				case(sf::Keyboard::A):
					this->keys[7] = 0x1;
					break;
				case(sf::Keyboard::S):
					this->keys[8] = 0x1;
					break;
				case(sf::Keyboard::D):
					this->keys[9] = 0x1;
					break;
				case(sf::Keyboard::Z):
					this->keys[10] = 0x1;
					break;
				case(sf::Keyboard::C):
					this->keys[11] = 0x1;
					break;
				case(sf::Keyboard::Num4):
					this->keys[12] = 0x1;
					break;
				case(sf::Keyboard::R):
					this->keys[13] = 0x1;
					break;
				case(sf::Keyboard::F):
					this->keys[14] = 0x1;
					break;
				case(sf::Keyboard::V):
					this->keys[15] = 0x1;
					break;
				default:
					break;
			}
		}
		if(this->event.type == sf::Event::KeyReleased)
		{
			switch(event.key.code)
			{
				case(sf::Keyboard::X):
					this->keys[0] = 0x0;
					break;
				case(sf::Keyboard::Num1):
					this->keys[1] = 0x0;
					break;
				case(sf::Keyboard::Num2):
					this->keys[2] = 0x0;
					break;
				case(sf::Keyboard::Num3):
					this->keys[3] = 0x0;
					break;
				case(sf::Keyboard::Q):
					this->keys[4] = 0x0;
					break;
				case(sf::Keyboard::W):
					this->keys[5] = 0x0;
					break;
				case(sf::Keyboard::E):
					this->keys[6] = 0x0;
					break;
				case(sf::Keyboard::A):
					this->keys[7] = 0x0;
					break;
				case(sf::Keyboard::S):
					this->keys[8] = 0x0;
					break;
				case(sf::Keyboard::D):
					this->keys[9] = 0x0;
					break;
				case(sf::Keyboard::Z):
					this->keys[10] = 0x0;
					break;
				case(sf::Keyboard::C):
					this->keys[11] = 0x0;
					break;
				case(sf::Keyboard::Num4):
					this->keys[12] = 0x0;
					break;
				case(sf::Keyboard::R):
					this->keys[13] = 0x0;
					break;
				case(sf::Keyboard::F):
					this->keys[14] = 0x0;
					break;
				case(sf::Keyboard::V):
					this->keys[15] = 0x0;
					break;
				default:
					break;
			}
		}

	}
}

void CHIP8::render()
{
	this->window->clear();
	this->graphics->render(this->window);
	this->window->display();
}

void CHIP8::update()
{
	this->updateSFMLEvents();
	this->dtClock->restart().asSeconds();	
	if(this->cpu->delay_timer > 0)
	{
		--this->cpu->delay_timer;
	}
	this->cpu->emulateCycle();
}

void CHIP8::loop() {
	while(this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

