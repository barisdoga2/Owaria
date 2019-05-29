#include <Map.h>


Map::Map(b2World* world) {

	// Load Map Game Objects
	sf::Vector2i tilemapPos;
	int objectSetCount, mapGameObjectCount;
	string objectsetName, gameobjectName;
	std::ifstream infile("../../Resources/Map/MapGameObjects.cfg");
	std::istringstream stream("");
	ioUtils::getNextLine(stream, infile);
	stream >> objectSetCount;
	for (int i = 0; i < objectSetCount; i++) {
		ioUtils::getNextLine(stream, infile);
		stream >> objectsetName;
		objectSets.push_back(new GameObjectSet(objectsetName));
	}
	ioUtils::getNextLine(stream, infile);
	stream >> mapGameObjectCount;
	for (int i = 0; i < mapGameObjectCount; i++) {
		ioUtils::getNextLine(stream, infile);
		stream >> objectsetName >> gameobjectName >> tilemapPos.x >> tilemapPos.y;
		for (GameObjectSet* g : objectSets) {
			if (g->getObjectsetName().compare(objectsetName) == 0) {
				gameObjects.push_back(new GameObject(g->getGameObjectData(gameobjectName), world, tilemapPos));
				break;
			}
		}

	}
	infile.close();

	// Load Tileset
	tileset = new Tileset("../../Resources/Map/Tileset.cfg", "../../Resources/Map/Tileset.png");
	
	// Load Tilemap
	int tmpTileID;
	infile.open("../../Resources/Map/Tilemap.cfg");
	ioUtils::getNextLine(stream, infile);
	stream >> mapTileSize.x >> mapTileSize.y;
	mapGridTileIDs = (Tile*)malloc(mapTileSize.x * mapTileSize.y * sizeof(Tile));
	for (int y = 0; y < mapTileSize.y; y++) {
		ioUtils::getNextLine(stream, infile);
		for (int x = 0; x < mapTileSize.x; x++) {
			stream >> tmpTileID;
			*(mapGridTileIDs + x + mapTileSize.x * y) = *tileset->getTile(tmpTileID);
		}
	}
	infile.close();


	// Load Map Buildings
	int buildingToLoad, mapBuildingCount, xTilemapPos, yTilemapPos;
	string buildingName;
	infile.open("../../Resources/Map/MapBuildings.cfg");
	ioUtils::getNextLine(stream, infile);
	stream >> buildingToLoad;
	for (int i = 0; i < buildingToLoad; i++) {
		ioUtils::getNextLine(stream, infile);
		stream >> buildingName;
		buildingDatas.push_back(new BuildingData(buildingName));
	}
	ioUtils::getNextLine(stream, infile);
	stream >> mapBuildingCount;
	for (int i = 0; i < mapBuildingCount; i++) {
		ioUtils::getNextLine(stream, infile);
		stream >> buildingName >> xTilemapPos >> yTilemapPos;
		for (BuildingData* bD : buildingDatas) {
			if (bD->buildingName.compare(buildingName) == 0) {
				new Building(bD, sf::Vector2i(xTilemapPos, yTilemapPos), this);
				break;
			}
		}

	}
	infile.close();

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

	delete tileset;

	for (GameObject* g : gameObjects)
		delete g;

	for(GameObjectSet* g : objectSets)
		delete g;
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
		sf::Vector2f tileSize = go->getGameObjectData()->gameObjectset->getTileset()->getTilePixelSize();
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
