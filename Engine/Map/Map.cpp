#include <Map.h>

Map::Map(b2World* world, const char* xml) {

	/*ASSET HANDLING*/
	XMLDocument tDoc;
	XMLElement* childIter;
	XMLElement* tMap;
	{
		// Open XML File
		tDoc.LoadFile(xml);

		// Get Map
		tMap = tDoc.FirstChildElement("Map");

		// Load Needed Tilesets
		childIter = tMap->FirstChildElement("Tilesets")->FirstChildElement("Tileset");
		while (childIter != nullptr) {
			AssetStore::LoadTileset(childIter);
			childIter = childIter->NextSiblingElement();
		}

		// Load Needed Building Assets
		childIter = tMap->FirstChildElement("BuildingAssets")->FirstChildElement("BuildingAsset");
		while (childIter != nullptr) {
			AssetStore::LoadBuildingAsset(childIter);
			childIter = childIter->NextSiblingElement();
		}

		// Load Needed Object Sets
		childIter = tMap->FirstChildElement("ObjectSets")->FirstChildElement("ObjectSet");
		while (childIter != nullptr) {
			AssetStore::LoadObjectSet(childIter);
			childIter = childIter->NextSiblingElement();
		}
	}
	
	// Create Map
	XMLElement* tileMap = tMap->FirstChildElement("Tilemap");

	// Set Tileset
	this->tileset = AssetStore::GetTileset(tileMap->Attribute("tilesetName"));

	// Load Tilemap
	this->mapTileSize = sf::Vector2i(tileMap->IntAttribute("width"), tileMap->IntAttribute("height"));
	istringstream str(tileMap->GetText());
	int tmpTileID;
	mapGridTileIDs = (Tile*)malloc(mapTileSize.x * mapTileSize.y * sizeof(Tile));
	for (int x = 0; x < mapTileSize.x * mapTileSize.y; x++) {
		str >> tmpTileID;
		*(mapGridTileIDs + x) = *tileset->getTile(tmpTileID);
	}

	// Load Map Buildings
	childIter = tMap->FirstChildElement("Buildings")->FirstChildElement("Building");
	while (childIter != nullptr) {
		gameBuildings.push_back(new Building(AssetStore::GetBuildingAsset(childIter->Attribute("buildingAssetName")), sf::Vector2i(atoi(childIter->FirstChildElement("tileMapXPos")->GetText()), atoi(childIter->FirstChildElement("tileMapYPos")->GetText()))));
		gameBuildings.at(gameBuildings.size() - 1)->mergeWithMap(this);
		childIter = childIter->NextSiblingElement();
	}

	// Load Map Objects
	childIter = tMap->FirstChildElement("Objects")->FirstChildElement("Object");
	while (childIter != nullptr) {
		gameObjects.push_back(new GameObject(AssetStore::GetObjectSet(childIter->Attribute("objectSetName"))->getGameObjectData(childIter->Attribute("objectDataName")), world, sf::Vector2i(atoi(childIter->FirstChildElement("tileMapXPos")->GetText()), atoi(childIter->FirstChildElement("tileMapYPos")->GetText()))));
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
	free(mapGridTileIDs);

	delete tileRenderer;

	for (GameObject* g : gameObjects)
		delete g;
	
	for (Building* b : gameBuildings)
		delete b;
}

void Map::Update(int updateElapsed) {
	
}

void Map::Render(sf::RenderWindow* window, Camera camera) {
	
	tileRenderer->setSize(tileset->getTilePixelSize());
	for (int y = 0; y < mapTileSize.y; y++) {
		for (int x = 0; x < mapTileSize.x; x++) {

			// Find Tile
			Tile* tile = (mapGridTileIDs + y * mapTileSize.x + x); // Tile 0 is transparent no need for rendering.
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
		Tile* t = go->getGameObjectData()->gameObjectTile;
		sf::Vector2f tileSize = go->getGameObjectData()->ObjectSet->getTileset()->getTilePixelSize();
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

sf::Vector2i Map::getMapTileSize() {
	return this->mapTileSize;
}

void Map::AddGameObject(GameObject* gameObject){
	gameObjects.push_back(gameObject);
}