#include <AssetStore.h>



std::vector<Tileset*> AssetStore::tilesets;
std::vector<BuildingAsset*> AssetStore::buildingAssets;
std::vector<ObjectSet*> AssetStore::objectSets;
std::vector<AnimationSet*> AssetStore::animationSets;
std::vector<ItemAsset*> AssetStore::itemAssets;

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

AnimationSet* AssetStore::GetAnimationSet(const char* animationsetName) {
	for (AnimationSet* t : animationSets)
		if (t->getName().compare(animationsetName) == 0)
			return t;

	return nullptr;
}

BuildingAsset* AssetStore::GetBuildingAsset(const char* buildingAssetName) {
	for (BuildingAsset* t : buildingAssets)
		if (t->getName().compare(buildingAssetName) == 0)
			return t;

	return nullptr;
}

ItemAsset* AssetStore::GetItemAsset(const char* itemAssetName) {
	for (ItemAsset* t : itemAssets)
		if (t->getName().compare(itemAssetName) == 0)
			return t;

	return nullptr;
}
ItemAsset* AssetStore::GetItemAssetByID(int id) {
	for (ItemAsset* t : itemAssets)
		if (t->getID() == id)
			return t;

	return nullptr;
}

void AssetStore::LoadTileset(XMLElement* tilesetElement) {
	if(GetTileset(tilesetElement->Attribute("name")) == nullptr)
		tilesets.push_back(new Tileset(tilesetElement));
}

void AssetStore::LoadObjectSet(XMLElement* objectsetElement) {
	if (GetObjectSet(objectsetElement->Attribute("name")) == nullptr)
		objectSets.push_back(new ObjectSet(objectsetElement));
}

void AssetStore::LoadBuildingAsset(XMLElement* buildingAssetElement) {
	if (GetBuildingAsset(buildingAssetElement->Attribute("name")) == nullptr)
		buildingAssets.push_back(new BuildingAsset(buildingAssetElement));
}

void AssetStore::LoadAnimationSet(XMLElement* animationsetElement) {
	if (GetAnimationSet(animationsetElement->Attribute("name")) == nullptr)
		animationSets.push_back(new AnimationSet(animationsetElement));
}

void AssetStore::LoadItemAsset(XMLElement* itemAssetElement) {
	if (GetItemAsset(itemAssetElement->Attribute("name")) == nullptr)
		itemAssets.push_back(new ItemAsset(itemAssetElement));
}

void AssetStore::CleanUp() {
	for (Tileset* t : tilesets)
		delete t;

	for (ObjectSet* g : objectSets)
		delete g;

	for (BuildingAsset* b : buildingAssets)
		delete b;

	for (AnimationSet* a : animationSets)
		delete a;

	for (ItemAsset* i : itemAssets)
		delete i;
}

vector<ObjectSet*> AssetStore::getAllObjectSets() {
	return objectSets;
}

vector<BuildingAsset*> AssetStore::getAllBuildingAssets() {
	return buildingAssets;
}

vector<AnimationSet*> AssetStore::getAllAnimationSets() {
	return animationSets;
}

vector<ItemAsset*> AssetStore::getAllItemAssets() {
	return itemAssets;
}