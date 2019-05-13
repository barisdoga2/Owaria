#include <Tile.h>

Tile::Tile(int id, sf::Image tileset, int xStart, int yStart, int pixelWidthTile, int pixelHeightTile, bool solid, TileData* tileData) {
	this->solid = solid;
	this->tileData = tileData;
	this->id = id;
	texture = new sf::Texture();
	texture->loadFromImage(tileset, sf::IntRect(xStart, yStart, pixelWidthTile, pixelHeightTile));
}

Tile::~Tile() {
	delete texture;
	delete tileData;
}

TileData* Tile::getTileData() {
	return this->tileData;
}

bool Tile::isSolid() {
	return this->solid;
}

int Tile::getID() {
	return this->id;
}

sf::Texture* Tile::getTexture() {
	return this->texture;
}