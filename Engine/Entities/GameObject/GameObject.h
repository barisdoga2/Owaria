#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <Tile.h>
class GameObjectSet;
#include <GameObjectSet.h>
#include <GameObjectData.h>

class GameObject {

public:
	GameObject(GameObjectData* gameObjectData, sf::Vector2i tilesetPos);
	~GameObject();

	GameObjectData* getGameObjectData();
	sf::Vector2i getTilemapPos();

private:
	GameObjectData* data;
	sf::Vector2i tilemapPos;

};