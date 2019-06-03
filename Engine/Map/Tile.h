#pragma once

#include <SFML/Graphics.hpp>
#include <TileAsset.h>


class Tile {
public:
	Tile(int id, sf::Image tileset, sf::Vector2f tilePosInTileset, sf::Vector2f tilePixelSize, int tilingPadding, bool solid, TileAsset* tileAsset);
	~Tile();

	int getID();
	bool isSolid();

	TileAsset* getTileAsset();
	sf::Texture* getTexture();

private:
	int id;
	bool solid;
	TileAsset* tileAsset;
	sf::Texture* texture;

};