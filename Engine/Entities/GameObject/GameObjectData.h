#pragma once

#include <iostream>
#include <Tile.h>
class GameObjectSet;
#include <GameObjectSet.h>

class GameObjectData {

public:
	GameObjectData() {}

	Tile* gameObjectTile;
	std::string gameObjectName;
	GameObjectSet* gameObjectset;
	bool hasPhysicsBody;
	bool isStatic;
	int contactDataType;
	int contactDataObject;
};