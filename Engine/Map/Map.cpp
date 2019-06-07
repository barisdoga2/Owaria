#include <Map.h>

Map::Map(b2World* world, const char* xml) {

	XMLDocument tDoc;
	tDoc.LoadFile(xml);
	XMLElement* tMap = tDoc.FirstChildElement("Map");

	// Load All Assets Related with Map
	XMLUtils::LoadMapAssets(tMap->FirstChildElement("Assets"));
	
	// Get Tilemap Element
	XMLElement* tileMap = tMap->FirstChildElement("Tilemap");

	// Set Tileset
	this->tileset = AssetStore::GetTileset(tileMap->Attribute("tilesetName"));

	// Load Tilemap
	this->mapSize = sf::Vector2i(tileMap->IntAttribute("width"), tileMap->IntAttribute("height"));
	istringstream str(tileMap->GetText());
	int tmpTileID;
	gridTiles = (Tile*)malloc(mapSize.x * mapSize.y * sizeof(Tile));
	for (int x = 0; x < mapSize.x * mapSize.y; x++) {
		str >> tmpTileID;
		*(gridTiles + x) = *tileset->getTile(tmpTileID);
	}

	// Load Map Buildings
	XMLElement* childIter;
	childIter = tMap->FirstChildElement("Buildings")->FirstChildElement("Building");
	while (childIter != nullptr) {
		gameBuildings.push_back(new Building(AssetStore::GetBuildingAsset(childIter->Attribute("buildingAssetName")), sf::Vector2i(atoi(childIter->FirstChildElement("tileMapXPos")->GetText()), atoi(childIter->FirstChildElement("tileMapYPos")->GetText()))));
		gameBuildings.at(gameBuildings.size() - 1)->mergeWithMap(this);
		childIter = childIter->NextSiblingElement();
	}

	// Load Map Objects
	childIter = tMap->FirstChildElement("Objects")->FirstChildElement("Object");
	while (childIter != nullptr) {
		gameObjects.push_back(new GameObject(AssetStore::GetObjectSet(childIter->Attribute("objectSetName"))->getObjectAsset(childIter->Attribute("objectDataName")), world, sf::Vector2i(atoi(childIter->FirstChildElement("tileMapXPos")->GetText()), atoi(childIter->FirstChildElement("tileMapYPos")->GetText()))));
		childIter = childIter->NextSiblingElement();
	}

	// Create Tile Renderer
	tileRenderer = new sf::RectangleShape();
	tileRenderer->setSize(tileset->getTilePixelSize());

	// Create Ground Fixture
	b2BodyDef m_b2BodyDef;
	m_b2BodyDef.type = b2_staticBody;
	m_b2BodyDef.position.Set(0, 0);
	body = world->CreateBody(&m_b2BodyDef);
	body->SetFixedRotation(true);

	// Create Marching Squares Physics
	ContactData* bodyContact = new ContactData(CONTACT_TYPE_MAP_INSTANCE, this);
	body->SetUserData((void*)bodyContact);
	marchingSquares = new MarchingSquares(this);
	for (MarchingSolution s : marchingSquares->solutions)
		b2Utils::AddChainLoopFixture(body, s.t_vertices, 0, 10, MAP_FRICTION, false)->SetUserData((void*)bodyContact);
}

Map::~Map() {
	free(gridTiles);
	
	delete tileRenderer;

	for (GameObject* g : gameObjects)
		delete g;
	
	for (Building* b : gameBuildings)
		delete b;
}

void Map::Update(int updateElapsed) {
	
}

void Map::Render(sf::RenderWindow* window, Camera camera) {
	sf::Vector2f cameraPos = camera.getPosition();

	int xStart = (int)cameraPos.x / (int)tileset->getTilesetTileSize().x;
	int yStart = (int)cameraPos.y / (int)tileset->getTilesetTileSize().y;
	
	tileRenderer->setSize(tileset->getTilePixelSize());
	for (int y = yStart; y < yStart + SCREEN_HEIGHT / tileset->getTilesetTileSize().y; y++) {
		for (int x = xStart; x < xStart + SCREEN_WIDTH / tileset->getTilesetTileSize().x + 1; x++) {

			// Find Tile
			Tile* tile = (gridTiles + y * mapSize.x + x); // Tile 0 is transparent no need for rendering.
			if (tile->getID() != 0) {
				// Render Tile
				tileRenderer->setTexture(tile->getTexture());
				tileRenderer->setPosition(sf::Vector2f((float)x * tileRenderer->getSize().x - camera.getPosition().x, (float)y * tileRenderer->getSize().y - camera.getPosition().y));
				window->draw(*tileRenderer);
			}
			
		}
	}

	// Render All Fixtures
	//b2Utils::RenderFixtures(window, body, m_offset);

	
	for (GameObject* go : gameObjects) {
		sf::RectangleShape r;
		Tile* t = go->getGameObjectData()->getTile();
		sf::Vector2f tileSize = go->getGameObjectData()->getObjectSet()->getTileset()->getTilePixelSize();
		r.setSize(tileSize);
		r.setTexture(t->getTexture());
		r.setPosition(sf::Vector2f(go->getTilemapPos().x * tileset->getTilePixelSize().x - camera.getPosition().x, go->getTilemapPos().y * tileset->getTilePixelSize().y - camera.getPosition().y));
		window->draw(r);
	}

}

void Map::HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin) {
	
}

Tileset* Map::getTileset() {
	return this->tileset;
}

sf::Vector2i Map::getMapSize() {
	return this->mapSize;
}