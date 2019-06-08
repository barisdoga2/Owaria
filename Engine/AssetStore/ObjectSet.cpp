#include <ObjectSet.h>


ObjectSet::ObjectSet(XMLElement* objectsetElement) {
	this->name = objectsetElement->Attribute("name");
	this->tileset = AssetStore::GetTileset(objectsetElement->Attribute("tilesetName"));

	XMLElement* object = objectsetElement->FirstChildElement("ObjectAsset");
	while (object != NULL) {
		objectAssets.push_back(new ObjectAsset(object->Attribute("name"), tileset->getTile(object->IntAttribute("tileSetTileID")), this, object->IntAttribute("hasPhysics"), object->IntAttribute("isStaticPhysics"), object->IntAttribute("contactDataType"), object->IntAttribute("contactDataObject")));
		object = object->NextSiblingElement();
	}
}

ObjectSet::~ObjectSet() {
	for (ObjectAsset* a : objectAssets)
		delete a;
}

ObjectAsset* ObjectSet::getObjectAsset(string name) {
	ObjectAsset* retVal = nullptr;
	for (ObjectAsset* a : objectAssets) 
		if (a->getName().compare(name) == 0) {
			retVal = a;
			break;
		}
		
	return retVal;
}

Tileset* ObjectSet::getTileset() {
	return this->tileset;
}

string ObjectSet::getName() {
	return this->name;
}

vector<ObjectAsset*> ObjectSet::getAllObjectAssets() {
	return this->objectAssets;
}