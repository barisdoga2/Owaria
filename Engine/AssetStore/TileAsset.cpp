#include <TileAsset.h>


TileAsset::TileAsset(int id, bool isXTile) {
	this->id = id;
	this->isXTile = isXTile;
}

void TileAsset::AppendVertex(XMLElement* vertex) {
	collision_vertices.push_back(sf::Vector2i(vertex->IntAttribute("x"), vertex->IntAttribute("y")));
}