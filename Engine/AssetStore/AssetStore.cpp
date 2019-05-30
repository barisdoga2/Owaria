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