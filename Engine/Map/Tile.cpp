#include <Tile.h>



Tile::Tile(int id, sf::Image tileset, sf::Vector2f tilePosInTileset, sf::Vector2f tilePixelSize, int tilingPadding, bool solid, TileData* tileData) {
	this->solid = solid;
	this->tileData = tileData;
	this->id = id;
	texture = new sf::Texture();
	texture->loadFromImage(tileset, sf::IntRect((int)(tilePosInTileset.x * (tilePixelSize.x + tilingPadding)), (int)(tilePosInTileset.y * (tilePixelSize.y + tilingPadding)), (int)tilePixelSize.x, (int)tilePixelSize.y));
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