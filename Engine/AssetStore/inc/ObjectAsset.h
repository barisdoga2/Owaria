#pragma once

#include <iostream>
#include <Tile.h>
class ObjectSet;
#include <ObjectSet.h>

class ObjectAsset {

public:
	ObjectAsset(std::string name, Tile* tile, ObjectSet* objectSet, bool hasPhysics, bool hasStaticPhysics, int contactDataType, int contactDataObject);

	std::string getName();
	Tile* getTile();
	ObjectSet* getObjectSet();
	bool isHasPhysics();
	bool isHasStaticPhysics();
	int getContactDataType();
	int getContactDataObject();

private:
	std::string name;
	Tile* tile;
	ObjectSet* objectSet;
	bool hasPhysics;
	bool hasStaticPhysics;
	int contactDataType;
	int contactDataObject;
};