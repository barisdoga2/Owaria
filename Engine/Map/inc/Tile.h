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

// Static Contex
public:
	static Tile* tileNotFound;
private:
	static void EarlyInit() {
		sf::Image tilesetIm;
		tilesetIm.loadFromFile("../../Resources/Icons/tileNotFound.png");
		tileNotFound = (Tile*)0xFFFFF; // To Block looping
		tileNotFound = new Tile(-1, tilesetIm, sf::Vector2f(0, 0), sf::Vector2f(16, 16), 0, 1, nullptr);
	}

};