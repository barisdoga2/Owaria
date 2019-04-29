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
		std::cout << line << std::endl;

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

	// Physics
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0 / BOX2D_SCALE, 100 / BOX2D_SCALE);
	body = world->CreateBody(&bodyDef);

	// Create Ground Fixture
	s.SetAsBox(100 / BOX2D_SCALE, 10 / BOX2D_SCALE);
	fixtureDef.density = 10.0f;
	fixtureDef.friction = 10.0f;
	fixtureDef.shape = &s;
	body->CreateFixture(&fixtureDef);

	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			int tileID = *(gridTileIDs + y * mapWidth + x);
			if (tileID == 0)
				continue;

			s.SetAsBox(16 / BOX2D_SCALE, 16 / BOX2D_SCALE);
			s.m_centroid = b2Vec2((x * singleTileWidth) / BOX2D_SCALE, (y * singleTileHeight) / BOX2D_SCALE);
			fixtureDef.shape = &s;

			//body->CreateFixture(&fixtureDef);
		}
	}

	
	// Dynamic Physics Body For Testing Purpose
	bodyDef2.type = b2_dynamicBody;
	bodyDef2.position.Set(0, 0);
	body2 = world->CreateBody(&bodyDef2);
	p.SetAsBox(16 / BOX2D_SCALE, 16 / BOX2D_SCALE);
	p.m_centroid = b2Vec2(0, 0);
	fixtureDef2.shape = &p;
	fix = body2->CreateFixture(&fixtureDef2);
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
			int tileID = *(gridTileIDs + y * mapWidth + x);

			// Find Tile
			Tile* tile;
			for(Tile* t : tiles)
				if (t->getID() == tileID) {
					tile = t;
					break;
				}

			// Render Tile
			tileRenderer->setTexture(tile->getTexture());
			tileRenderer->setPosition(sf::Vector2f((float)x * tileRenderer->getSize().x, (float)y * tileRenderer->getSize().y));
			window->draw(*tileRenderer);
		}
	}

	// Render Dynamic Physics Object For Testing
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(16, 16));
	rect.setPosition(body2->GetPosition().x * BOX2D_SCALE, body2->GetPosition().y * BOX2D_SCALE);
	rect.setFillColor(sf::Color::Red);
	window->draw(rect);
}