#include "Graphics.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include<iostream>

void Graphics::render(sf::RenderWindow* window)
{

	for(int i = 0; i<32;i++)
	{
		for(int j = 0; j<64;j++)
		{
			if(this->gfx[i*64+j]==0xFF)
			{
			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f(10,10));
			rect.setFillColor(sf::Color::White);
			rect.setPosition(j*10,  i*10);
			window->draw(rect);
			}
		}
	}
}
