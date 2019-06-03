#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Tile.h>
#include <Tileset.h>
class ObjectAsset;
#include <ObjectAsset.h>
#include <AssetStore.h>

using namespace std;

class ObjectSet {

public:

public:
	ObjectSet(XMLElement* objectsetElement);
	~ObjectSet();

	Tileset* getTileset();
	ObjectAsset* getObjectAsset(string name);

	string getObjectsetName();
	string getName();
	
private:
	string name;
	string gameObjectPrefix;
	Tileset* tileset;
	vector<ObjectAsset*> objectAssets;

};

