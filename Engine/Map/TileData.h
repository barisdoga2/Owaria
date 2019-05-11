#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class TileData {

public:
	int id;
	bool isXTile;
	std::vector<sf::Vector2i>* collision_vertices;

	TileData::TileData(int id, bool isXTile, std::vector<sf::Vector2i>* collision_vertices) {
		this->id = id;
		this->isXTile = isXTile;
		this->collision_vertices = collision_vertices;
	}

};