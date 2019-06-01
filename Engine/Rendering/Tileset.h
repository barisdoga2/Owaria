#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <Tile.h>

using namespace std;

class Tileset {

public:
	Tileset(XMLElement* tilesetElement);
	~Tileset();

	Tile* getTile(int id);
	sf::Vector2f getTilesetTileSize();
	sf::Vector2f getTilePixelSize();

	string getName();

private:
	string name;

	sf::Image* tilesetImage;
	sf::Vector2f tilesetSize;
	sf::Vector2f tilePixelSize;
	int tilingPadding;

	vector<Tile*> tiles;
};