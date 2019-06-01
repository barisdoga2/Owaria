#include <ObjectAsset.h>

ObjectAsset::ObjectAsset(std::string name, Tile* tile, ObjectSet* objectSet, bool hasPhysics, bool hasStaticPhysics, int contactDataType, int contactDataObject) {
	this->name = name;
	this->tile = tile;
	this->objectSet = objectSet;
	this->hasPhysics = hasPhysics;
	this->hasStaticPhysics = hasStaticPhysics;
	this->contactDataType = contactDataType;
	this->contactDataObject = contactDataObject;
}

std::string ObjectAsset::getName() {
	return name;
}

Tile* ObjectAsset::getTile() {
	return tile;
}

ObjectSet* ObjectAsset::getObjectSet() {
	return objectSet;
}

bool ObjectAsset::isHasPhysics() {
	return hasPhysics;
}

bool ObjectAsset::isHasStaticPhysics() {
	return hasStaticPhysics;
}

int ObjectAsset::getContactDataType() {
	return contactDataType;
}

int ObjectAsset::getContactDataObject() {
	return contactDataObject;
}