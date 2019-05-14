#pragma once

#include <SFML/Graphics.hpp>
#include <TileData.h>

class Tile {
public:
	Tile(int id, sf::Image tileset, int xStart, int yStart, sf::Vector2f tilePixelSize, bool solid, TileData* tileData);
	~Tile();
	int getID();
	bool isSolid();
	TileData* getTileData();
	sf::Texture* getTexture();
private:
	int id;
	bool solid;
	TileData* tileData;
	sf::Texture* texture;
};