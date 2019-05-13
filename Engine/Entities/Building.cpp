#include <Building.h>



Building::Building(int left, int top, Map* map) {
	this->left = left;
	this->top = top;

	std::string buildingPath = "../../Resources/Building/";
	std::string tilesetName = "Tileset";
	std::string buildingName = "Building";
	std::string tilemapName = "Tilemap";

	// Load Tileset
	tileset = new sf::Image();
	tileset->loadFromFile(buildingPath + tilesetName + ".png");

	// Load Tileset Config
	int tilesetWidth, tilesetHeight, singleTileWidth, singleTileHeight, tilingPadding, tmp;
	std::ifstream infile(buildingPath + tilesetName + ".cfg");
	std::istringstream stream("");

	ioUtils::getNextLine(stream, infile);
	stream >> tilesetWidth >> tilesetHeight >> singleTileWidth >> singleTileHeight >> tilingPadding;

	int solidTileIDsLength;
	ioUtils::getNextLine(stream, infile);
	stream >> solidTileIDsLength;

	vector<int> solidTileIDs;
	ioUtils::getNextLine(stream, infile);
	for (int i = 0; i < solidTileIDsLength; i++) {
		stream >> tmp;
		solidTileIDs.push_back(tmp);
	}

	vector<TileData*> tileDatas;
	int specialCollisionTileCount, id, isXTile, verticeCount, x, y;
	ioUtils::getNextLine(stream, infile);
	stream >> specialCollisionTileCount;
	for (int i = 0; i < specialCollisionTileCount; i++) {
		ioUtils::getNextLine(stream, infile);
		stream >> id >> isXTile >> verticeCount;
		vector<sf::Vector2i>* collision_vertices = new vector<sf::Vector2i>;
		for (int j = 0; j < verticeCount; j++) {
			ioUtils::getNextLine(stream, infile);
			stream >> x >> y;
			collision_vertices->push_back(sf::Vector2i(x, y));
		}
		tileDatas.push_back(new TileData(id, isXTile == 1, collision_vertices));
	}

	infile.close();

	// Create Tiles
	int idCtr = 0;
	for (int y = 0; y < tilesetHeight; y++) {
		for (int x = 0; x < tilesetWidth; x++) {
			bool f = false;
			for (int i : solidTileIDs)
				if (i == idCtr) {
					f = true;
					break;
				}
			TileData* tileData = nullptr;
			for (TileData* tile : tileDatas)
				if (tile->id == idCtr) {
					tileData = tile;
					break;
				}
			tiles.push_back(new Tile(idCtr, *tileset, x * (singleTileWidth + tilingPadding), y * (singleTileHeight + tilingPadding), singleTileWidth, singleTileHeight, f, tileData));
			idCtr++;
		}
	}

	// Load Map Config
	std::string line;
	infile.open(buildingPath + buildingName + ".cfg");
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		if (line[0] == '#')
			continue;
		if (iss >> widthTiles >> heightTiles)
			break;
	}
	infile.close();

	// Load Tilemap
	infile.open(buildingPath + tilemapName + ".cfg");
	mapGridTileIDs = (Tile*)malloc(widthTiles * heightTiles * sizeof(Tile));
	int currentColumn = 0;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		if (line[0] == '#')
			continue;
		for (int x = 0; x < widthTiles; x++) {
			int tileID;
			iss >> tileID;
			*(mapGridTileIDs + x + widthTiles * currentColumn) = *getTile(tileID);
		}

		currentColumn++;
	}
	infile.close();

	for (int y = 0; y < heightTiles; y++) {
		for (int x = 0; x < widthTiles; x++) {
			
		}
	}

	for (int y = top; y < top + heightTiles; y++)
		for (int x = left; x < left + widthTiles; x++)
			*(map->mapGridTileIDs + y * map->getMapWidth() + x) = *(mapGridTileIDs + (y - top) * widthTiles + x - left);
}

Building::~Building() {
	free(mapGridTileIDs);

	delete tileset;

	for (Tile* t : tiles)
		delete t;
}

Tile* Building::getTile(int id) {
	for (Tile* tile : tiles)
		if (tile->getID() == id)
			return tile;
	return nullptr;
}