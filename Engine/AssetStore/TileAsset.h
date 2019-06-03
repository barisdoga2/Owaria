#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <tinyxml2.h>

using namespace tinyxml2;

class TileAsset {

public:
	TileAsset::TileAsset(int id, bool isXTile);
	void AppendVertex(XMLElement* vertex);

	int id;
	bool isXTile;
	std::vector<sf::Vector2i> collision_vertices;

};