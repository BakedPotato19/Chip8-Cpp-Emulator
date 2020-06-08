#include"SFML/Graphics.hpp"
#include"SFML/Window.hpp"
#include"SFML/Audio.hpp"
#include"SFML/System.hpp"
#include"SFML/Network.hpp"
class Graphics
{
	private:

	public:
		unsigned char gfx[64*32];

		void render(sf::RenderWindow* window);
};
