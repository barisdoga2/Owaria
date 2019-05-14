#pragma once

#include <SFML/Graphics.hpp>
#include <TileData.h>


class Tile {
public:
	Tile(int id, sf::Image tileset, sf::Vector2f tilePosInTileset, sf::Vector2f tilePixelSize, int tilingPadding, bool solid, TileData* tileData);
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