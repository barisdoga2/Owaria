#include <Map.h>


Map::Map(b2World* world) {
	std::string mapPath = "../../Resources/Map/";	
	std::string tilesetName = "Tileset2";
	std::string mapName = "Map2";
	std::string tilemapName = "Tilemap2";

	// Load Tileset
	tileset = new sf::Image();
	tileset->loadFromFile(mapPath + tilesetName + ".png");

	// Load Tileset Config
	int tilesetWidth, tilesetHeight, singleTileWidth, singleTileHeight, tilingPadding, tmp;
	std::ifstream infile(mapPath + tilesetName + ".cfg");
	std::istringstream stream("");

	ioUtils::getNextLine(stream, infile);
	stream >> tilesetWidth >> tilesetHeight >> singleTileWidth >> singleTileHeight >> tilingPadding;

	int solidTileIDsLength;
	ioUtils::getNextLine(stream, infile);
	stream >> solidTileIDsLength;

	vector<int> solidTileIDs;
	ioUtils::getNextLine(stream, infile);
	for (int i = 0; i < solidTileIDsLength; i++) {
		stream >> tmp;
		solidTileIDs.push_back(tmp);
	}
	
	vector<TileData*> tileDatas;
	int specialCollisionTileCount, id, isXTile, verticeCount, x, y;
	ioUtils::getNextLine(stream, infile);
	stream >> specialCollisionTileCount;
	for (int i = 0; i < specialCollisionTileCount; i++) {
		ioUtils::getNextLine(stream, infile);
		stream >> id >> isXTile >> verticeCount;
		vector<sf::Vector2i>* collision_vertices = new vector<sf::Vector2i>;
		for (int j = 0; j < verticeCount; j++) {
			ioUtils::getNextLine(stream, infile);
			stream >> x >> y;
			collision_vertices->push_back(sf::Vector2i(x, y));
		}
		tileDatas.push_back(new TileData(id, isXTile == 1, collision_vertices));
	}

	infile.close();

	// Create Tiles
	int idCtr = 0;
	for (int y = 0; y < tilesetHeight; y++) {
		for (int x = 0; x < tilesetWidth; x++) {
			bool f = false;
			for(int i : solidTileIDs)
				if (i == idCtr) {
					f = true;
					break;
				}
			TileData* tileData = nullptr;
			for(TileData* tile : tileDatas)
				if (tile->id == idCtr) {
					tileData = tile;
					break;
				}
			tiles.push_back(new Tile(idCtr, *tileset, x * (singleTileWidth + tilingPadding), y * (singleTileHeight + tilingPadding), singleTileWidth, singleTileHeight, f, tileData));
			idCtr++;
		}
	}

	// Load Map Config
	std::string line;
	infile.open(mapPath + mapName + ".cfg");
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
	infile.open(mapPath + tilemapName + ".cfg");
	mapGridTileIDs = (Tile*)malloc(mapWidth * mapHeight * sizeof(Tile));
	int currentColumn = 0;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		if (line[0] == '#')
			continue;
		for (int x = 0; x < mapWidth; x++) {
			int tileID;
			iss >> tileID;
			*(mapGridTileIDs + x + mapWidth * currentColumn) = *getTile(tileID);
		}
			
		currentColumn++;
	}
	infile.close();

	// Tile Renderer
	tileRenderer = new sf::RectangleShape();
	tileRenderer->setSize(sf::Vector2f((float)singleTileWidth, (float)singleTileHeight));

	// Create Building
	building = new Building(90, 26, this);

	// Create Ground Fixture
	b2BodyDef m_b2BodyDef;
	m_b2BodyDef.type = b2_staticBody;
	m_b2BodyDef.position.Set(0, 0);
	body = world->CreateBody(&m_b2BodyDef);
	body->SetFixedRotation(true);

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			Tile* tile = (mapGridTileIDs + y * mapWidth + x);
			if (tile == 0) // Tile 0 is transparent, and not collidable.
				continue;
			// Until now no physics check needed for tiles
			//b2Fixture* ff = b2Utils::AddRectangleFixture(body, 16, 16, x * 16, y * 16, 0, 0, 0, true);
		}
	}

	// Create Marching Squares Physics
	ContactData* bodyContact = new ContactData(CONTACT_TYPE_MAP_INSTANCE, this);

	body->SetUserData((void*)bodyContact);

	marchingSquares = new MarchingSquares(this);
	for (MarchingSolution s : marchingSquares->solutions)
		b2Utils::AddChainLoopFixture(body, s.t_vertices, 0, 10, MAP_FRICTION, false)->SetUserData((void*)bodyContact);
}

Map::~Map() {
	delete tileset;

	for (Tile* t : tiles)
		delete t;

	free(mapGridTileIDs);

	delete tileRenderer;
	delete building;
}

void Map::Update(int updateElapsed) {
	
}

void Map::Render(sf::RenderWindow* window) {
	
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {

			// Find Tile
			Tile* tile = (mapGridTileIDs + y * mapWidth + x); // Tile 0 is transparent no need for rendering.
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

int Map::getMapWidth() {
	return mapWidth;
}

int Map::getMapHeight() {
	return mapHeight;
}

void Map::HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin) {
	
}

Tile* Map::getTile(int id) {
	for (Tile* tile : tiles)
		if (tile->getID() == id)
			return tile;
	return nullptr;
}