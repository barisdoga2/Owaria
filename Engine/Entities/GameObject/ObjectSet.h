#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <ioUtils.h>
#include <Tile.h>
#include <Tileset.h>
class GameObjectData;
#include <GameObjectData.h>

using namespace std;



class ObjectSet {

public:

public:
	ObjectSet(string objectsetName, Tileset* tileset, const char* cfg);
	~ObjectSet();

	Tileset* getTileset();
	GameObjectData* getGameObjectData(string name);

	string getObjectsetName();
	string getName();
	
private:
	string objectsetName;
	string gameObjectPrefix;
	Tileset* tileset;
	vector<GameObjectData*> gameobjectdatas;

};

