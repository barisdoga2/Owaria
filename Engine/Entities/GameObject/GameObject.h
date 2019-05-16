#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <Tile.h>
class GameObjectSet;
#include <GameObjectSet.h>
#include <GameObjectData.h>

class GameObject {

public:
	GameObject(GameObjectData* gameObjectData, b2World* world, sf::Vector2i tilesetPos);
	~GameObject();

	GameObjectData* getGameObjectData();
	sf::Vector2i getTilemapPos();

private:
	b2Body* objectBody;
	GameObjectData* data;
	sf::Vector2i tilemapPos;

};