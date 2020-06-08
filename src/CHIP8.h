#include "MEMORY.h"
#include "CPU.h"
#include<cstring>
#include<fstream>

class CHIP8
{
	private:

		// Defines the CHIP8
		// 0x keyboard.

		Memory* memory;
		CPU* cpu;
		Graphics* graphics;
		unsigned char keys[16];
		

		void setupGraphics();
		void setupInput();
		void loadFontsetToMemory();
		void loadRomToMemory(std::string path);
		void loop();



		sf::Event event;
		sf::RenderWindow* window;
		sf::Clock* dtClock;


	public:

		unsigned char* chip8_fontset;
		void initialize();	
		void render();
		void update();
		void updateSFMLEvents();
		unsigned char processKeyEvent();


};
