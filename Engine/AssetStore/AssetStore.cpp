#include <AssetStore.h>



std::vector<Tileset*> AssetStore::tilesets;
std::vector<BuildingAsset*> AssetStore::buildingAssets;
std::vector<ObjectSet*> AssetStore::objectSets;

Tileset* AssetStore::GetTileset(const char* tilesetName) {
	for (Tileset* t : tilesets) 
		if (t->getName().compare(tilesetName) == 0) 
			return t;
	
	return nullptr;
}

ObjectSet* AssetStore::GetObjectSet(const char* objectsetName) {
	for (ObjectSet* t : objectSets)
		if (t->getName().compare(objectsetName) == 0)
			return t;

	return nullptr;
}

BuildingAsset* AssetStore::GetBuildingAsset(const char* buildingAssetName) {
	for (BuildingAsset* t : buildingAssets)
		if (t->getName().compare(buildingAssetName) == 0)
			return t;

	return nullptr;
}

void AssetStore::LoadTileset(XMLElement* tilesetElement) {
	// Load Solid TileIDs
	vector<int> solidIDs;
	XMLNode* solidIDsNode = tilesetElement->FirstChildElement("solidIDs");
	if (solidIDsNode != NULL) {
		string solidIDsStr = tilesetElement->FirstChildElement("solidIDs")->GetText();
		int tmpID;
		std::istringstream iss(solidIDsStr);
		while (true) {
			if (!(iss >> tmpID))
				break;
			else
				solidIDs.push_back(tmpID);
		}
	}
	
	// Load Special Tiles
	vector<TileData*> tileDatas;
	XMLNode* tileDatasNode = tilesetElement->FirstChildElement("specialTiles");
	if (tileDatasNode != NULL) {
		XMLElement* specialTile = tilesetElement->FirstChildElement("specialTiles")->FirstChildElement("specialTile");
		while (specialTile != nullptr) {
			TileData* tileData = new TileData(specialTile->IntAttribute("id"), false);

			XMLElement* vertex = specialTile->FirstChildElement("vertex");
			while (vertex != nullptr) {
				tileData->AppendVertex(vertex);
				vertex = vertex->NextSiblingElement();
			}

			tileDatas.push_back(tileData);
			specialTile = specialTile->NextSiblingElement();
		}
	}
	
	// Create Tileset
	tilesets.push_back(new Tileset(tilesetElement->Attribute("name"), tilesetElement->FirstChildElement("png")->GetText(), sf::Vector2f(atoi(tilesetElement->FirstChildElement("width")->GetText()), atoi(tilesetElement->FirstChildElement("height")->GetText())), sf::Vector2f(atoi(tilesetElement->FirstChildElement("tilePixWidth")->GetText()), atoi(tilesetElement->FirstChildElement("tilePixHeight")->GetText())), atoi(tilesetElement->FirstChildElement("padding")->GetText()), solidIDs, tileDatas));
}

void AssetStore::LoadObjectSet(XMLElement* objectsetElement) {
	ObjectSet* objectSet = new ObjectSet(objectsetElement->Attribute("name"), GetTileset(objectsetElement->Attribute("tilesetName")));

	XMLElement* object = objectsetElement->FirstChildElement("ObjectData");
	while (object != NULL) {
		objectSet->AppendObjectData(object);
		object = object->NextSiblingElement();
	}

	objectSets.push_back(objectSet);
}

void AssetStore::LoadBuildingAsset(XMLElement* buildingAssetElement) {
	XMLElement* tilemap = buildingAssetElement->FirstChildElement("Tilemap");
	buildingAssets.push_back(new BuildingAsset(buildingAssetElement->Attribute("name"), GetTileset(buildingAssetElement->Attribute("tilesetName")), sf::Vector2i(tilemap->IntAttribute("width"), tilemap->IntAttribute("height")), tilemap->GetText()));
}

void AssetStore::CleanUp() {
	for (Tileset* t : tilesets)
		delete t;

	for (ObjectSet* g : objectSets)
		delete g;

	for (BuildingAsset* b : buildingAssets)
		delete b;
}