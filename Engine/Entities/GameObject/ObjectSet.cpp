#include <ObjectSet.h>


ObjectSet::ObjectSet(string objectsetName, Tileset* tileset, const char* cfg) {
	this->objectsetName = objectsetName;
	this->tileset = tileset;

	string gameObjectSuffix;
	int numberOfObjects, id, hasPhysicsBody, contactDataType, contactDataObject, isStatic;
	std::ifstream infile(cfg);
	std::istringstream stream("");
	ioUtils::getNextLine(stream, infile);
	stream >> numberOfObjects >> gameObjectPrefix;
	for (int i = 0; i < numberOfObjects; i++) {
		ioUtils::getNextLine(stream, infile);

		stream >> id >> gameObjectSuffix >> hasPhysicsBody >> isStatic >> contactDataType >> contactDataObject;
		GameObjectData* gameObjData = new GameObjectData();
		gameObjData->gameObjectTile = tileset->getTile(id);
		gameObjData->gameObjectName = gameObjectPrefix + gameObjectSuffix;
		gameObjData->ObjectSet = this;
		gameObjData->hasPhysicsBody = hasPhysicsBody;
		gameObjData->isStatic = isStatic;
		gameObjData->contactDataType = contactDataType;
		gameObjData->contactDataObject = contactDataObject;
		
		gameobjectdatas.push_back(gameObjData);
	}
	infile.close();
	
}

ObjectSet::~ObjectSet() {
	for (GameObjectData* g : gameobjectdatas)
		delete g;
}

GameObjectData* ObjectSet::getGameObjectData(string name) {
	GameObjectData* retVal = nullptr;
	for (GameObjectData* god : gameobjectdatas) 
		if (god->gameObjectName.compare(name) == 0) {
			retVal = god;
			break;
		}
		
	return retVal;
}

Tileset* ObjectSet::getTileset() {
	return this->tileset;
}

string ObjectSet::getObjectsetName() {
	return this->objectsetName;
}

string ObjectSet::getName() {
	return this->objectsetName;
}