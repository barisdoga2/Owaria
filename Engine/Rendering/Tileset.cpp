#include <Tileset.h>
#include <iostream>

Tileset::Tileset(const char* tilesetName, const char* pngPath, sf::Vector2f size, sf::Vector2f tilePixSize, int tilingPadding, vector<int> solidTileIDs, vector<TileData*> tileDatas) {
	this->name = tilesetName;
	this->tilesetTileSize = size;
	this->tilePixelSize = tilePixSize;
	this->tilingPadding = tilingPadding;

	this->tilesetImage = new sf::Image();
	this->tilesetImage->loadFromFile(pngPath);

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