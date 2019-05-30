#pragma once

#include <iostream>
#include <Tile.h>
class ObjectSet;
#include <ObjectSet.h>

class GameObjectData {

public:
	GameObjectData() {}

	Tile* gameObjectTile;
	std::string gameObjectName;
	ObjectSet* ObjectSet;
	bool hasPhysicsBody;
	bool isStatic;
	int contactDataType;
	int contactDataObject;
};