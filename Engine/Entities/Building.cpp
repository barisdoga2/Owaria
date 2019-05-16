#include <Building.h>



Building::Building(BuildingData* buildingData, sf::Vector2i buildingTilePos, Map* map) {
	this->data = buildingData;
	this->buildingTilePos = buildingTilePos;


	for (int y = buildingTilePos.y; y < buildingTilePos.y + buildingData->tilemapSize.y; y++)
		for (int x = buildingTilePos.x; x < buildingTilePos.x + buildingData->tilemapSize.x; x++) {
			*(map->mapGridTileIDs + y * map->getMapTileSize().x + x) = *(buildingData->mapGridTileIDs + (y - buildingTilePos.y) * buildingData->tilemapSize.x + x - buildingTilePos.x);
		}
}

Building::~Building() {
	/*
	free(mapGridTileIDs);

	delete tileset;
	*/
}

sf::Vector2i Building::getTilePosOnMap() {
	return buildingTilePos;
}