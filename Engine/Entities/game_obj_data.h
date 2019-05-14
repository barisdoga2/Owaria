#pragma once

#include <iostream>
#include <Tile.h>
class GameObjectSet;
#include <GameObjectSet.h>

class game_obj_data {

public:
	game_obj_data() {}
	Tile* gameObjectTile;
	std::string gameObjectName;
	GameObjectSet* gameObjectset;
};