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
class AnimationSet;
#include <AnimationSet.h>

using namespace std;
using namespace tinyxml2;

class AssetStore {

public:
	static void CleanUp();
	static Tileset* GetTileset(const char* tilesetName);
	static ObjectSet* GetObjectSet(const char* objectsetName);
	static BuildingAsset* GetBuildingAsset(const char* buildingAssetName);
	static AnimationSet* GetAnimationSet(const char* animationsetName);

	static void LoadTileset(XMLElement* tilesetElement);
	static void LoadObjectSet(XMLElement* objectsetElement);
	static void LoadBuildingAsset(XMLElement* buildingAssetElement);
	static void LoadAnimationSet(XMLElement* animationsetElement);

	static vector<ObjectSet*> getAllObjectSets();
	static vector<BuildingAsset*> getAllBuildingAssets();
	static vector<AnimationSet*> getAllAnimationSets();

private:
	AssetStore();
	static std::vector<Tileset*> tilesets;
	static std::vector<BuildingAsset*> buildingAssets;
	static std::vector<ObjectSet*> objectSets;
	static std::vector<AnimationSet*> animationSets;

};