#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <tinyxml2.h>
class Tileset;
#include <Tileset.h>
class BuildingAsset;
#include <BuildingAsset.h>
class ObjectSet;
#include <ObjectSet.h>

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

	static vector<ObjectSet*> getAllObjectSets();

private:
	AssetStore();
	static std::vector<Tileset*> tilesets;
	static std::vector<BuildingAsset*> buildingAssets;
	static std::vector<ObjectSet*> objectSets;

};