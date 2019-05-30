#include <Tileset.h>
#include <iostream>

Tileset::Tileset(string tilesetName, string tilesetPng, string tilesetConfig) {
	// Load Tileset
	this->name = tilesetName;
	tilesetImage = new sf::Image();
	tilesetImage->loadFromFile(tilesetPng);

	// Load Tileset Config
	std::ifstream infile(tilesetConfig);
	std::istringstream stream("");
	ioUtils::getNextLine(stream, infile);
	stream >> tilesetTileSize.x >> tilesetTileSize.y >> tilePixelSize.x >> tilePixelSize.y >> tilingPadding;
	
	// Load Solid Tile IDs
	int solidTileIDsLength, tmp;
	vector<int> solidTileIDs;
	ioUtils::getNextLine(stream, infile);
	stream >> solidTileIDsLength;
	ioUtils::getNextLine(stream, infile);
	for (int i = 0; i < solidTileIDsLength; i++) {
		stream >> tmp;
		solidTileIDs.push_back(tmp);
	}

	// Load Speical Collision Tile Datas
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
	for (int y = 0; y < tilesetTileSize.y; y++) {
		for (int x = 0; x < tilesetTileSize.x; x++) {
			// Check if is id solid
			bool isSolid = false;
			for (int i : solidTileIDs)
				if (i == idCtr) {
					isSolid = true;
					break;
				}
			
			// Check if is id have special collision data
			TileData* tileData = nullptr;
			for (TileData* tile : tileDatas)
				if (tile->id == idCtr) {
					tileData = tile;
					break;
				}

			// Create tile
			tiles.push_back(new Tile(idCtr, *tilesetImage, sf::Vector2f((float)x, (float)y), tilePixelSize, tilingPadding, isSolid, tileData));
			idCtr++;
		}
	}
}

Tileset::~Tileset() {
	delete tilesetImage;
	for (Tile* t : tiles)
		delete t;
}

Tile* Tileset::getTile(int id) {
	for (Tile* tile : tiles)
		if (tile->getID() == id)
			return tile;
	return nullptr;
}

sf::Vector2f Tileset::getTilesetTileSize() {
	return this->tilesetTileSize;
}

sf::Vector2f Tileset::getTilePixelSize() {
	return this->tilePixelSize;
}

string Tileset::getName() {
	return this->name;
}