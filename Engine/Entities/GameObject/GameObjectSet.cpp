#include <GameObjectSet.h>


GameObjectSet::GameObjectSet(string objectsetName) {
	this->objectsetName = objectsetName;
	string objectSetPath = "../../Resources/GameObjects/" + objectsetName;
	tileset = new Tileset(objectSetPath + "/Tileset.cfg", objectSetPath  + "/Tileset.png");
	
	string gameObjectSuffix;
	int numberOfObjects, id, hasPhysicsBody, contactDataType, contactDataObject, isStatic;
	std::ifstream infile(objectSetPath + "/" + objectsetName + ".cfg");
	std::istringstream stream("");
	ioUtils::getNextLine(stream, infile);
	stream >> numberOfObjects >> gameObjectPrefix;
	for (int i = 0; i < numberOfObjects; i++) {
		ioUtils::getNextLine(stream, infile);

		stream >> id >> gameObjectSuffix >> hasPhysicsBody >> isStatic >> contactDataType >> contactDataObject;
		GameObjectData* gameObjData = new GameObjectData();
		gameObjData->gameObjectTile = tileset->getTile(id);
		gameObjData->gameObjectName = gameObjectPrefix + gameObjectSuffix;
		gameObjData->gameObjectset = this;
		gameObjData->hasPhysicsBody = hasPhysicsBody;
		gameObjData->isStatic = isStatic;
		gameObjData->contactDataType = contactDataType;
		gameObjData->contactDataObject = contactDataObject;
		
		gameobjectdatas.push_back(gameObjData);
	}
	infile.close();
	
}

GameObjectSet::~GameObjectSet() {
	delete tileset;

	for (GameObjectData* g : gameobjectdatas)
		delete g;
}

GameObjectData* GameObjectSet::getGameObjectData(string name) {
	GameObjectData* retVal = nullptr;
	for (GameObjectData* god : gameobjectdatas) 
		if (god->gameObjectName.compare(name) == 0) {
			retVal = god;
			break;
		}
		
	return retVal;
}

Tileset* GameObjectSet::getTileset() {
	return this->tileset;
}

string GameObjectSet::getObjectsetName() {
	return this->objectsetName;
}