#include <Map.h>

Map::Map(b2World* world, sf::RenderWindow* window, const char* xml) {
	this->world = world;

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
	ApplyMarchingSquares();

	// Create Editors
	tilemapEditor = new TilemapEditor(window, this);
	objectEditor = new ObjectEditor(window, this);
	buildingEditor = new BuildingEditor(window, this);
}

Map::~Map() {
	free(gridTiles);
	
	delete tileRenderer;

	for (GameObject* g : gameObjects)
		delete g;
	
	for (Building* b : gameBuildings)
		delete b;

	delete tilemapEditor;
	delete objectEditor;
	delete buildingEditor;
}

void Map::Update(int updateElapsed, Camera* camera) {
	tilemapEditor->Update(updateElapsed, camera);
	objectEditor->Update(updateElapsed, camera);
	buildingEditor->Update(updateElapsed, camera);
}

void Map::Render(sf::RenderWindow* window, Camera* camera) {
	sf::Vector2f cameraPos = camera->getPosition();

	int xStart = (int)floor(cameraPos.x / tileset->getTilePixelSize().x);
	int yStart = (int)floor(cameraPos.y / tileset->getTilePixelSize().y);

	int xEnd = xStart + (int)ceil(SCREEN_WIDTH / tileset->getTilePixelSize().x) + 1;
	int yEnd = yStart + (int)ceil(SCREEN_HEIGHT / tileset->getTilePixelSize().y) + 1;
	xEnd = xEnd > mapSize.x ? mapSize.x : xEnd;
	yEnd = yEnd > mapSize.y ? mapSize.y : yEnd;
	
	tileRenderer->setSize(tileset->getTilePixelSize());
	for (int y = yStart; y < yEnd; y++) {
		for (int x = xStart; x < xEnd; x++) {

			// Find Tile
			Tile* tile = (gridTiles + y * mapSize.x + x); // Tile 0 is transparent no need for rendering.
			if (tile->getID() != 0) {
				// Render Tile
				tileRenderer->setTexture(tile->getTexture());
				tileRenderer->setPosition(sf::Vector2f((float)x * tileRenderer->getSize().x - camera->getPosition().x, (float)y * tileRenderer->getSize().y - camera->getPosition().y));
				window->draw(*tileRenderer);
			}
			
		}
	}

	for (GameObject* go : gameObjects) {
		sf::RectangleShape r;
		Tile* t = go->getGameObjectData()->getTile();
		sf::Vector2f tileSize = go->getGameObjectData()->getObjectSet()->getTileset()->getTilePixelSize();
		r.setSize(tileSize);
		r.setTexture(t->getTexture());
		r.setPosition(sf::Vector2f(go->getTilemapPos().x * tileset->getTilePixelSize().x - camera->getPosition().x, go->getTilemapPos().y * tileset->getTilePixelSize().y - camera->getPosition().y));
		window->draw(r);
	}

	tilemapEditor->Render(camera);
	objectEditor->Render(camera);
	buildingEditor->Render(camera);
}

void Map::HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin) {
	
}

Tileset* Map::getTileset() {
	return this->tileset;
}

sf::Vector2i Map::getMapSize() {
	return this->mapSize;
}

void Map::HandleWindowEvent(sf::Event event, Camera* camera) {
	tilemapEditor->HandleWindowEvent(event, camera);
	objectEditor->HandleWindowEvent(event, camera);
	buildingEditor->HandleWindowEvent(event, camera);
}

void Map::ApplyMarchingSquares() {
	ContactData* bodyContact = new ContactData(CONTACT_TYPE_MAP_INSTANCE, (void*)MAP_SENSOR);
	body->SetUserData((void*)bodyContact);
	marchingSquares = new MarchingSquares(this);
	
	for (b2Fixture* m : marchingSquaresFixtures) 
		body->DestroyFixture(m);
	marchingSquaresFixtures.clear();
	for (MarchingSolution s : marchingSquares->solutions) {
		b2Fixture* m = b2Utils::AddChainLoopFixture(body, s.t_vertices, 0, 10, MAP_FRICTION, false);
		m->SetUserData((void*)bodyContact);
		marchingSquaresFixtures.push_back(m);
	}
		
}

void Map::AddGameObject(ObjectAsset* asset, sf::Vector2i tilePos) {
	gameObjects.push_back(new GameObject(asset, world, tilePos));
}

void Map::AddBuilding(BuildingAsset* asset, sf::Vector2i tilePos) {
	gameBuildings.push_back(new Building(asset, tilePos));
	gameBuildings.at(gameBuildings.size() - 1)->mergeWithMap(this);
}