#include <GameObjectSet.h>


GameObjectSet::GameObjectSet(string objectSetPath) {
		
	tileset = new Tileset(objectSetPath + "/Tileset.cfg", objectSetPath  + "/Tileset.png");
	
	string gameObjectSuffix;
	int numberOfObjects, id;
	std::ifstream infile(objectSetPath + "/Tree.cfg");
	std::istringstream stream("");
	ioUtils::getNextLine(stream, infile);
	stream >> numberOfObjects >> gameObjectPrefix;
	for (int i = 0; i < numberOfObjects; i++) {
		ioUtils::getNextLine(stream, infile);

		stream >> id >> gameObjectSuffix;
		game_obj_data* gameObjData = new game_obj_data();
		gameObjData->gameObjectTile = tileset->getTile(id);
		gameObjData->gameObjectName = gameObjectPrefix + gameObjectSuffix;
		gameObjData->gameObjectset = this;
		gameobjectdatas.push_back(gameObjData);
	}
	infile.close();
	
}

GameObjectSet::~GameObjectSet() {
	delete tileset;

	for (game_obj_data* g : gameobjectdatas)
		delete g;
}

game_obj_data* GameObjectSet::getGameObjectData(string name) {
	game_obj_data* retVal = nullptr;
	for (game_obj_data* god : gameobjectdatas)
		if (god->gameObjectName.compare(name) > 0) {
			retVal = god;
			break;
		}
	return retVal;
}

Tileset* GameObjectSet::getTileset() {
	return this->tileset;
}