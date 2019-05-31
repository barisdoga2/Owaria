#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Tileset.h>
#include <BuildingAsset.h>
#include <GameObjectData.h>
#include <ObjectSet.h>
#include <tinyxml2.h>
#include <cstdlib>

using namespace std;
using namespace tinyxml2;

class AssetStore {

public:
	static void CleanUp();
	static Tileset* GetTileset(const char* tilesetName);
	static ObjectSet* GetObjectSet(string objectsetName);
	static BuildingAsset* GetBuildingAsset(string buildingAssetName);

	static void LoadTileset(const char* tilesetName, const char* png, const char* cfg);
	static void LoadTileset(XMLElement* tilesetElement);
	static void LoadObjectSet(const char* objectsetName, const char* tilesetName, const char* cfg);
	static void LoadBuildingAsset(const char* buildingAssetName, const char* tilesetName, const char* cfg);

private:
	AssetStore();
	static std::vector<Tileset*> tilesets;
	static std::vector<BuildingAsset*> buildingAssets;
	static std::vector<ObjectSet*> objectSets;



};