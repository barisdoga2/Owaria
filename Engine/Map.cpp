#include <Map.h>


Map::Map(b2World* world) {
	std::string mapPath = "../../Resources/Map/";	

	// Load Tileset
	tileset = new sf::Image();
	tileset->loadFromFile(mapPath + "Tileset.png");

	// Load Tileset Config
	int tilesetWidth, tilesetHeight, singleTileWidth, singleTileHeight, tilingPadding;
	std::ifstream infile(mapPath + "Tileset.cfg");
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		if (line[0] == '#')
			continue;
		if (iss >> tilesetWidth >> tilesetHeight >> singleTileWidth >> singleTileHeight >> tilingPadding)
			break;
	}
	infile.close();

	// Create Tiles
	int idCtr = 0;
	for (int y = 0; y < tilesetHeight; y++) {
		for (int x = 0; x < tilesetWidth; x++) {
			tiles.push_back(new Tile(idCtr++, *tileset, x * (singleTileWidth + tilingPadding), y * (singleTileHeight + tilingPadding), singleTileWidth, singleTileHeight));
		}
	}

	// Load Map Config
	infile.open(mapPath + "Map.cfg");
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		if (line[0] == '#')
			continue;
		if (iss >> mapWidth >> mapHeight)
			break;
	}
	infile.close();

	// Load Tilemap
	infile.open(mapPath + "Tilemap.cfg");
	gridTileIDs = (int*)malloc(mapWidth * mapHeight * sizeof(int));
	int currentColumn = 0;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		if (line[0] == '#')
			continue;
		for (int x = 0; x < mapWidth; x++)
			iss >> *(gridTileIDs + x + mapWidth * currentColumn);
		currentColumn++;
	}
	infile.close();

	// Tile Renderer
	tileRenderer = new sf::RectangleShape();
	tileRenderer->setSize(sf::Vector2f(singleTileWidth, singleTileHeight));

	// Create Ground Fixture
	b2BodyDef m_b2BodyDef;
	m_b2BodyDef.type = b2_staticBody;
	m_b2BodyDef.position.Set(0, 0);
	body = world->CreateBody(&m_b2BodyDef);
	body->SetFixedRotation(true);

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			int tileID = *(gridTileIDs + y * mapWidth + x);
			if (tileID == 0) // Tile 0 is transparent, and not collidable.
				continue;
			b2Fixture* ff = AddRectangleFixture(8, 8, x * 16 + 16 / 2, y * 16 + 16 / 2, 0, 0, 0);
		}
	}
}

Map::~Map() {
	delete tileset;

	for (Tile* t : tiles)
		delete t;

	free(gridTileIDs);

	delete tileRenderer;
}

void Map::Update(int updateElapsed) {
	
}

void Map::Render(sf::RenderWindow* window) {
	
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {

			// Find Tile ID
			int tileID = *(gridTileIDs + y * mapWidth + x); // Tile 0 is transparent no need for rendering.
			if (tileID == 0)
				continue;

			// Find Tile
			Tile* tile;
			for(Tile* t : tiles)
				if (t->getID() == tileID) {
					tile = t;
					break;
				}

			// Render Tile
			tileRenderer->setTexture(tile->getTexture());
			tileRenderer->setPosition(sf::Vector2f((float)x * tileRenderer->getSize().x - m_offset.x, (float)y * tileRenderer->getSize().y - m_offset.y));
			//window->draw(*tileRenderer);
		}
	}
	

	// Render All Fixtures
	b2Fixture* fix = body->GetFixtureList();
	sf::Color renderColor = sf::Color(125, 125, 125, 125);
	while (fix != nullptr) {
		if (b2PolygonShape* v = dynamic_cast<b2PolygonShape*>(fix->GetShape())) {

			sf::VertexArray vertices(sf::TrianglesFan, v->GetVertexCount());
			for (int i = 0; i < v->GetVertexCount(); i++)
				vertices[i] = sf::Vertex(sf::Vector2f(v->m_vertices[i].x * BOX2D_SCALE - m_offset.x, v->m_vertices[i].y * BOX2D_SCALE - m_offset.y), renderColor);

			window->draw(vertices);
		}
		fix = fix->GetNext();
	}
}

int Map::getMapWidth() {
	return mapWidth;
}

int Map::getMapHeight() {
	return mapHeight;
}

b2Fixture* Map::AddRectangleFixture(int width, int height, int x, int y, float restitution, float density, float friction) {
	b2PolygonShape polygonShape;
	b2FixtureDef fixtureDef;
	polygonShape.SetAsBox(width / BOX2D_SCALE, height / BOX2D_SCALE, b2Vec2(x / BOX2D_SCALE, y / BOX2D_SCALE), 0);
	fixtureDef.restitution = restitution;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.shape = &polygonShape;
	return body->CreateFixture(&fixtureDef);
}