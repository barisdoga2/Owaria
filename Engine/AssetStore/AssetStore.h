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
	static ObjectSet* GetObjectSet(const char* objectsetName);
	static BuildingAsset* GetBuildingAsset(const char* buildingAssetName);

	static void LoadTileset(XMLElement* tilesetElement);
	static void LoadObjectSet(XMLElement* objectsetElement);
	static void LoadBuildingAsset(XMLElement* buildingAssetElement);

private:
	AssetStore();
	static std::vector<Tileset*> tilesets;
	static std::vector<BuildingAsset*> buildingAssets;
	static std::vector<ObjectSet*> objectSets;



};