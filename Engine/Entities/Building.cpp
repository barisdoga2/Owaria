#include <Building.h>



Building::Building(sf::Vector2i buildingTilePos, Map* map) {
	this->buildingTilePos = buildingTilePos;

	// Load Tileset
	tileset = new Tileset("../../Resources/Building/Tileset.cfg", "../../Resources/Building/Tileset.png");

	// Load Tilemap
	int tmpTileID;
	std::ifstream infile("../../Resources/Building/Tilemap.cfg");
	std::istringstream stream("");
	ioUtils::getNextLine(stream, infile);
	stream >> buildingTileSize.x >> buildingTileSize.y;
	mapGridTileIDs = (Tile*)malloc(buildingTileSize.x * buildingTileSize.y * sizeof(Tile));
	for (int y = 0; y < buildingTileSize.y; y++) {
		ioUtils::getNextLine(stream, infile);
		for (int x = 0; x < buildingTileSize.x; x++) {
			stream >> tmpTileID;
			*(mapGridTileIDs + x + buildingTileSize.x * y) = *tileset->getTile(tmpTileID);
		}
	}
	infile.close();

	for (int y = buildingTilePos.y; y < buildingTilePos.y + buildingTileSize.y; y++)
		for (int x = buildingTilePos.x; x < buildingTilePos.x + buildingTileSize.x; x++)
			*(map->mapGridTileIDs + y * map->getMapTileSize().x + x) = *(mapGridTileIDs + (y - buildingTilePos.y) * buildingTileSize.x + x - buildingTilePos.x);
}

Building::~Building() {
	free(mapGridTileIDs);

	delete tileset;
}