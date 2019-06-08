#include <TilemapEditor.h>



TilemapEditor::TilemapEditor(sf::RenderWindow* window, Map* map) {
	cout << "Press 'CTRL+T' or '1' to open or close Tilemap Editor! While it is opened you can press 'H' to hide or show tile selector!" << endl;
	this->window = window;
	this->map = map;

	gui = new Gui(*window);

	sf::Texture tt;
	tt.loadFromImage(*map->getTileset()->getImage());
	tilesetImage = Picture::create(tt, false);
	tilesetImage->connect("clicked", (std::function<void()>)std::bind(&TilemapEditor::clickCallback, this));
	
	gui->add(tilesetImage);

	selectedTileRenderer = new sf::RectangleShape();
	selectedTileRenderer->setSize(map->getTileset()->getTilePixelSize());
}

TilemapEditor::~TilemapEditor() {
	delete gui;
	delete selectedTileRenderer;
}

void TilemapEditor::Update(int updateElapsed, Camera* camera) {
	static bool keyRelaseFlag = false;
	if (((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) && sf::Keyboard::isKeyPressed(sf::Keyboard::T)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))) {
		if (keyRelaseFlag) {
			isActive = !isActive;
			selectedTile = nullptr;
			tilesetImage->setVisible(true);
			camera->SetFreeRoam(isActive);
			keyRelaseFlag = false;
		}
	}
	else {
		keyRelaseFlag = true;
	}

	if (!isActive)
		return;

	static bool keyRelaseFlag2 = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
		if (keyRelaseFlag2) {
			tilesetImage->setVisible(!tilesetImage->isVisible());
			keyRelaseFlag2 = false;
		}
	}
	else {
		keyRelaseFlag2 = true;
	}

}

void TilemapEditor::Render(Camera* camera) {
	if (!isActive)
		return;
	gui->draw();
	
	if (selectedTile != nullptr) {
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);
		sf::Vector2f cPos = camera->getPosition();
		sf::Vector2i tilePos = sf::Vector2i((int)mPos.x / (int)map->getTileset()->getTilePixelSize().x, (int)mPos.y / (int)map->getTileset()->getTilePixelSize().y);

		selectedTileRenderer->setTexture(selectedTile->getTexture());
		selectedTileRenderer->setPosition(tilePos.x * map->getTileset()->getTilePixelSize().x - mathUtils::findMod(cPos.x, map->getTileset()->getTilePixelSize().x), tilePos.y * map->getTileset()->getTilePixelSize().y - mathUtils::findMod(cPos.y, map->getTileset()->getTilePixelSize().y));
		window->draw(*selectedTileRenderer);
	}
	
}

void TilemapEditor::HandleWindowEvent(sf::Event event, Camera* camera) {
	if (!isActive)
		return;

	gui->handleEvent(event);

	sf::Vector2i mPos = sf::Mouse::getPosition(*window);
	if (selectedTile == nullptr || (tilesetImage->isVisible() && mathUtils::intersectsWith(tilesetImage->getPosition(), map->getTileset()->getImage()->getSize(), mPos)))
		return;

	static bool mouseRelaseFlag = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2f cPos = camera->getPosition();
		sf::Vector2i tilePos = sf::Vector2i((int)(cPos.x + mPos.x) / (int)map->getTileset()->getTilePixelSize().x, (int)(cPos.y + mPos.y) / (int)map->getTileset()->getTilePixelSize().y);

		*(map->gridTiles + tilePos.x + tilePos.y * map->getMapSize().x) = *selectedTile;

		if (mouseRelaseFlag) 
			mouseRelaseFlag = false;
	}
	else {
		if (!mouseRelaseFlag) 
			map->reMarch();
		mouseRelaseFlag = true;
	}
	
}

void TilemapEditor::clickCallback() {
	sf::Vector2i mPos = sf::Mouse::getPosition(*window);
	sf::Vector2f iPos = tilesetImage->getPosition();
	sf::Vector2i absPos = sf::Vector2i(mPos.x - (int)iPos.x, mPos.y - (int)iPos.y);
	sf::Vector2i tilePos = sf::Vector2i((int)absPos.x / ((int)map->getTileset()->getTilePixelSize().x + map->getTileset()->getTilingPadding()), (int)absPos.y / ((int)map->getTileset()->getTilePixelSize().y + map->getTileset()->getTilingPadding()));
	int tileID = tilePos.x + tilePos.y * (int)map->getTileset()->getTilesetTileSize().x;
	selectedTile = map->getTileset()->getTile(tileID);
}