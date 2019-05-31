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

ObjectSet* AssetStore::GetObjectSet(string objectsetName) {
	for (ObjectSet* t : objectSets)
		if (t->getName() == objectsetName)
			return t;

	return nullptr;
}

BuildingAsset* AssetStore::GetBuildingAsset(string buildingAssetName) {
	for (BuildingAsset* t : buildingAssets)
		if (t->getName() == buildingAssetName)
			return t;

	return nullptr;
}

void AssetStore::LoadTileset(const char* tilesetName, const char* png, const char* cfg) {
	tilesets.push_back(new Tileset(tilesetName, png, cfg));
}

void AssetStore::LoadTileset(XMLElement* tilesetElement) {
	// Load Solid TileIDs
	vector<int> solidIDs;
	if (tilesetElement->FirstChildElement("solidIDs")) {
		cout << "a" << tilesetElement->Attribute("name");
	}
	else
		cout << "b";
	string solidIDsStr = tilesetElement->FirstChildElement("solidIDs")->GetText();
	if (solidIDsStr.length() > 0) {
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
	/*
	XMLElement* specialTile = tilesetElement->FirstChildElement("specialTiles")->FirstChildElement("specialTile");
	while (specialTile != nullptr) {
		vector<sf::Vector2i> vertices;
		XMLElement* vertex = specialTile->FirstChildElement("vertex");
		while (vertex != nullptr) {
			vertices.push_back(sf::Vector2i(vertex->IntAttribute("x"), vertex->IntAttribute("y")));
			vertex = vertex->NextSiblingElement();
		}
		tileDatas.push_back(new TileData(specialTile->IntAttribute("id"), false, &vertices));
		specialTile = specialTile->NextSiblingElement();
	}
	*/
	tilesets.push_back(new Tileset(tilesetElement->Attribute("name"), tilesetElement->FirstChildElement("png")->GetText(), sf::Vector2f(atoi(tilesetElement->FirstChildElement("width")->GetText()), atoi(tilesetElement->FirstChildElement("height")->GetText())), sf::Vector2f(atoi(tilesetElement->FirstChildElement("tilePixWidth")->GetText()), atoi(tilesetElement->FirstChildElement("tilePixHeight")->GetText())), atoi(tilesetElement->FirstChildElement("padding")->GetText()), solidIDs, tileDatas));
}


void AssetStore::LoadObjectSet(const char* objectsetName, const char* tilesetName, const char* cfg) {
	objectSets.push_back(new ObjectSet(objectsetName, GetTileset(tilesetName), cfg));
}

void AssetStore::LoadBuildingAsset(const char* buildingAssetName, const char* tilesetName, const char* cfg) {
	buildingAssets.push_back(new BuildingAsset(buildingAssetName, GetTileset(tilesetName), cfg));
}

void AssetStore::CleanUp() {
	for (Tileset* t : tilesets)
		delete t;

	for (ObjectSet* g : objectSets)
		delete g;

	for (BuildingAsset* b : buildingAssets)
		delete b;
}