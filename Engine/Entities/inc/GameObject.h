#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <Tile.h>
class ObjectSet;
#include <ObjectSet.h>
#include <ObjectAsset.h>

class GameObject {

public:
	GameObject(ObjectAsset* gameObjectData, b2World* world, sf::Vector2i tilesetPos);
	~GameObject();

	ObjectAsset* getGameObjectData();
	sf::Vector2i getTilemapPos();

private:
	b2Body* objectBody;
	ObjectAsset* data;
	sf::Vector2i tilemapPos;

};