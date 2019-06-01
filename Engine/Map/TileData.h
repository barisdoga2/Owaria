#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <tinyxml2.h>

using namespace tinyxml2;

class TileData {

public:
	int id;
	bool isXTile;
	std::vector<sf::Vector2i> collision_vertices;

	TileData::TileData(int id, bool isXTile) {
		this->id = id;
		this->isXTile = isXTile;
	}

	void TileData::AppendVertex(XMLElement* vertex) {
		collision_vertices.push_back(sf::Vector2i(vertex->IntAttribute("x"), vertex->IntAttribute("y")));
	}

};