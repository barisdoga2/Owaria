#pragma once

#include <tinyxml2.h>
#include <AssetStore.h>

using namespace tinyxml2;

class XMLUtils {
public:
	static void LoadMapAssets(XMLElement* assetsElement) {
		XMLElement* childIter;
	
		// Load Needed Tilesets
		childIter = assetsElement->FirstChildElement("Tilesets")->FirstChildElement("Tileset");
		while (childIter != nullptr) {
			AssetStore::LoadTileset(childIter);
			childIter = childIter->NextSiblingElement();
		}

		// Load Needed Building Assets
		childIter = assetsElement->FirstChildElement("BuildingAssets")->FirstChildElement("BuildingAsset");
		while (childIter != nullptr) {
			AssetStore::LoadBuildingAsset(childIter);
			childIter = childIter->NextSiblingElement();
		}

		// Load Needed Object Sets
		childIter = assetsElement->FirstChildElement("ObjectSets")->FirstChildElement("ObjectSet");
		while (childIter != nullptr) {
			AssetStore::LoadObjectSet(childIter);
			childIter = childIter->NextSiblingElement();
		}
	}
};