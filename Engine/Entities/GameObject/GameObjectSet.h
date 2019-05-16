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
#include <GameObjectData.h>

using namespace std;



class GameObjectSet {

public:

public:
	GameObjectSet(string objectsetName);
	~GameObjectSet();

	Tileset* getTileset();
	GameObjectData* getGameObjectData(string name);

	string getObjectsetName();
	
private:
	string objectsetName;
	string gameObjectPrefix;
	Tileset* tileset;
	vector<GameObjectData*> gameobjectdatas;

};

