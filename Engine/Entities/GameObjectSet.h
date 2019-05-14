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
#include <game_obj_data.h>

using namespace std;



class GameObjectSet {

public:

public:
	GameObjectSet(string objectSetPath);
	~GameObjectSet();

	Tileset* getTileset();
	game_obj_data* getGameObjectData(string name);
	
private:
	string gameObjectPrefix;
	Tileset* tileset;
	vector<game_obj_data*> gameobjectdatas;

};

