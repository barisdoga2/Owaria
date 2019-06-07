#include <Tileset.h>
#include <iostream>

Tileset::Tileset(XMLElement* tilesetElement) {
	this->name = tilesetElement->Attribute("name");
	this->tilesetSize = sf::Vector2f((float)atoi(tilesetElement->FirstChildElement("width")->GetText()), (float)atoi(tilesetElement->FirstChildElement("height")->GetText()));
	this->tilePixelSize = sf::Vector2f((float)atoi(tilesetElement->FirstChildElement("tilePixWidth")->GetText()), (float)atoi(tilesetElement->FirstChildElement("tilePixHeight")->GetText()));
	this->tilingPadding = atoi(tilesetElement->FirstChildElement("padding")->GetText());
	this->tilesetImage = new sf::Image();
	this->tilesetImage->loadFromFile(tilesetElement->FirstChildElement("png")->GetText());

	// Load Solid IDs
	vector<int> solidTileIDs;
	XMLNode* solidIDsNode = tilesetElement->FirstChildElement("solidIDs");
	if (solidIDsNode != NULL) {
		string solidIDsStr = tilesetElement->FirstChildElement("solidIDs")->GetText();
		int tmpID;
		std::istringstream iss(solidIDsStr);
		while (true) {
			if (!(iss >> tmpID))
				break;
			else
				solidTileIDs.push_back(tmpID);
		}
	}

	// Load Special Tiles
	vector<TileAsset*> tileDatas;
	XMLNode* tileDatasNode = tilesetElement->FirstChildElement("specialTiles");
	if (tileDatasNode != NULL) {
		XMLElement* specialTile = tilesetElement->FirstChildElement("specialTiles")->FirstChildElement("specialTile");
		while (specialTile != nullptr) {
			TileAsset* tileAsset = new TileAsset(specialTile->IntAttribute("id"), false);

			XMLElement* vertex = specialTile->FirstChildElement("vertex");
			while (vertex != nullptr) {
				tileAsset->AppendVertex(vertex);
				vertex = vertex->NextSiblingElement();
			}

			tileDatas.push_back(tileAsset);
			specialTile = specialTile->NextSiblingElement();
		}
	}

	// Create Tiles
	int idCtr = 0;
	for (int y = 0; y < tilesetSize.y; y++) {
		for (int x = 0; x < tilesetSize.x; x++) {
			// Check if is id solid
			bool isSolid = false;
			for (int i : solidTileIDs)
				if (i == idCtr) {
					isSolid = true;
					break;
				}

			// Check if is id have special collision data
			TileAsset* tileAsset = nullptr;
			for (TileAsset* tile : tileDatas)
				if (tile->id == idCtr) {
					tileAsset = tile;
					break;
				}

			// Create tile
			tiles.push_back(new Tile(idCtr, *tilesetImage, sf::Vector2f((float)x, (float)y), tilePixelSize, tilingPadding, isSolid, tileAsset));
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

	return Tile::tileNotFound;
}

sf::Vector2f Tileset::getTilesetTileSize() {
	return this->tilesetSize;
}

sf::Vector2f Tileset::getTilePixelSize() {
	return this->tilePixelSize;
}

string Tileset::getName() {
	return this->name;
}