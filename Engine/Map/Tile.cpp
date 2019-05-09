#include <Tile.h>

Tile::Tile(int id, sf::Image tileset, int xStart, int yStart, int pixelWidthTile, int pixelHeightTile) {
	this->id = id;
	texture = new sf::Texture();
	texture->loadFromImage(tileset, sf::IntRect(xStart, yStart, pixelWidthTile, pixelHeightTile));
}

Tile::~Tile() {
	delete texture;
}

int Tile::getID() {
	return this->id;
}

sf::Texture* Tile::getTexture() {
	return this->texture;
}