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
	tilesets.push_back(new Tileset(tilesetElement));
}

void AssetStore::LoadObjectSet(XMLElement* objectsetElement) {
	objectSets.push_back(new ObjectSet(objectsetElement));
}

void AssetStore::LoadBuildingAsset(XMLElement* buildingAssetElement) {
	buildingAssets.push_back(new BuildingAsset(buildingAssetElement));
}

void AssetStore::CleanUp() {
	for (Tileset* t : tilesets)
		delete t;

	for (ObjectSet* g : objectSets)
		delete g;

	for (BuildingAsset* b : buildingAssets)
		delete b;
}

vector<ObjectSet*> AssetStore::getAllObjectSets() {
	return objectSets;
}