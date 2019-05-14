#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <Tile.h>
class GameObjectSet;
#include <GameObjectSet.h>

class GameObject {

public:
	GameObject(game_obj_data* gameObjectData, sf::Vector2i tilesetPos);
	~GameObject();

	game_obj_data* getGameObjectData();
	sf::Vector2i getTilemapPos();

private:
	game_obj_data* data;
	sf::Vector2i tilemapPos;

};