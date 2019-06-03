#include <BuildingAsset.h>



BuildingAsset::BuildingAsset(XMLElement* buildingAssetElement) {
	this->buildingName = buildingAssetElement->Attribute("name");
	this->tileset = AssetStore::GetTileset(buildingAssetElement->Attribute("tilesetName"));

	XMLElement* tilemap = buildingAssetElement->FirstChildElement("Tilemap");
	this->tilemapSize = sf::Vector2i(tilemap->IntAttribute("width"), tilemap->IntAttribute("height"));

	// Load Tilemap
	int tmpTileID;
	std::istringstream stream(tilemap->GetText());
	mapGridTileIDs = (Tile*)malloc(tilemapSize.x * tilemapSize.y * sizeof(Tile));
	for (int x = 0; x < tilemapSize.y * tilemapSize.x; x++) {
		stream >> tmpTileID;
		*(mapGridTileIDs + x) = *tileset->getTile(tmpTileID);
	}
}

BuildingAsset::~BuildingAsset() {
	free(mapGridTileIDs);
}

string BuildingAsset::getName() {
	return buildingName;
}