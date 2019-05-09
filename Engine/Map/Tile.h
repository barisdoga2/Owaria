#pragma once

#include <SFML/Graphics.hpp>

class Tile {
public:
	Tile(int id, sf::Image tileset, int xStart, int yStart, int pixelWidthTile, int pixelHeightTile);
	~Tile();
	int getID();
	sf::Texture* getTexture();
private:
	int id;
	sf::Texture* texture;
};