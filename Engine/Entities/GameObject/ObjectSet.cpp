#include <ObjectSet.h>


ObjectSet::ObjectSet(string objectsetName, Tileset* tileset) {
	this->objectsetName = objectsetName;
	this->tileset = tileset;
}

ObjectSet::~ObjectSet() {
	for (GameObjectData* g : gameobjectdatas)
		delete g;
}

void ObjectSet::AppendObjectData(XMLElement* objectElement) {
	GameObjectData* gameObjectData = new GameObjectData();
	gameObjectData->gameObjectTile = tileset->getTile(objectElement->IntAttribute("tileSetTileID"));
	gameObjectData->gameObjectName = objectElement->Attribute("name");
	gameObjectData->ObjectSet = this;
	gameObjectData->hasPhysicsBody = objectElement->IntAttribute("hasPhysics");
	gameObjectData->isStatic = objectElement->IntAttribute("isStaticPhysics");
	gameObjectData->contactDataType = objectElement->IntAttribute("contactDataType");
	gameObjectData->contactDataObject = objectElement->IntAttribute("contactDataObject");

	gameobjectdatas.push_back(gameObjectData);
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