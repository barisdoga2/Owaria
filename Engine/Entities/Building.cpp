#include <Building.h>



Building::Building(BuildingAsset* BuildingAsset, sf::Vector2i buildingTilePos) {
	this->data = BuildingAsset;
	this->buildingTilePos = buildingTilePos;
}

Building::~Building() {
	
}

void Building::mergeWithMap(Map* map) {
	// Push Building Tilemap to Map's Tilemap
	for (int y = buildingTilePos.y; y < buildingTilePos.y + data->tilemapSize.y; y++)
		for (int x = buildingTilePos.x; x < buildingTilePos.x + data->tilemapSize.x; x++)
			*(map->gridTiles + y * map->getMapSize().x + x) = *(data->mapGridTileIDs + (y - buildingTilePos.y) * data->tilemapSize.x + x - buildingTilePos.x);
}

sf::Vector2i Building::getTilePosOnMap() {
	return buildingTilePos;
}