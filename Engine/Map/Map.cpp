#include <Map.h>


Map::Map(b2World* world) {

	// Load Tileset
	tileset = new Tileset("../../Resources/Map/Tileset.cfg", "../../Resources/Map/Tileset.png");
	
	// Load Tilemap
	int tmpTileID;
	std::ifstream infile("../../Resources/Map/Tilemap.cfg");
	std::istringstream stream("");
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

	// Create Tile Renderer
	tileRenderer = new sf::RectangleShape();
	tileRenderer->setSize(sf::Vector2f(tileset->getTilePixelSize().x, tileset->getTilePixelSize().y));

	// Create Building
	building = new Building(sf::Vector2i(90, 26), this);

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
	delete building;

	delete tileset;
}

void Map::Update(int updateElapsed) {
	
}

void Map::Render(sf::RenderWindow* window) {
	
	for (int y = 0; y < mapTileSize.y; y++) {
		for (int x = 0; x < mapTileSize.x; x++) {

			// Find Tile
			Tile* tile = (mapGridTileIDs + y * mapTileSize.x + x); // Tile 0 is transparent no need for rendering.
			if (tile->getID() != 0) {
				// Render Tile
				tileRenderer->setTexture(tile->getTexture());
				tileRenderer->setPosition(sf::Vector2f((float)x * tileRenderer->getSize().x - m_offset.x, (float)y * tileRenderer->getSize().y - m_offset.y));
				window->draw(*tileRenderer);
			}
			
		}
	}

	// Render All Fixtures
	//b2Utils::RenderFixtures(window, body, m_offset);
}

void Map::HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin) {
	
}

Tileset* Map::getTileset() {
	return this->tileset;
}

sf::Vector2i Map::getMapTileSize() {
	return this->mapTileSize;
}