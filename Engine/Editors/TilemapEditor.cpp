#include <TilemapEditor.h>



TilemapEditor::TilemapEditor(sf::RenderWindow* window, Tileset* tileset) {
	cout << "Press 'CTRL+T' or '1' to open or close Tilemap Editor! While it is opened you can press 'H' to hide or show tile selector!" << endl;
	this->window = window;
	this->tileset = tileset;

	gui = new Gui(*window);

	sf::Texture tt;
	tt.loadFromImage(*tileset->getImage());
	tilesetImage = Picture::create(tt, false);
	tilesetImage->connect("clicked", (std::function<void()>)std::bind(&TilemapEditor::clickCallback, this));
	
	gui->add(tilesetImage);

	selectedTileRenderer = new sf::RectangleShape();
	selectedTileRenderer->setSize(tileset->getTilePixelSize());
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
		sf::Vector2i tilePos = sf::Vector2i((int)mPos.x / (int)tileset->getTilePixelSize().x, (int)mPos.y / (int)tileset->getTilePixelSize().y);

		selectedTileRenderer->setTexture(selectedTile->getTexture());
		selectedTileRenderer->setPosition(tilePos.x * tileset->getTilePixelSize().x - mathUtils::findMod(cPos.x, tileset->getTilePixelSize().x), tilePos.y * tileset->getTilePixelSize().y - mathUtils::findMod(cPos.y, tileset->getTilePixelSize().y));
		window->draw(*selectedTileRenderer);
	}
	
}

void TilemapEditor::HandleWindowEvent(sf::Event event) {
	if (!isActive)
		return;

	gui->handleEvent(event);

	
}

void TilemapEditor::clickCallback() {
	sf::Vector2i mPos = sf::Mouse::getPosition(*window);
	sf::Vector2f iPos = tilesetImage->getPosition();
	sf::Vector2i absPos = sf::Vector2i(mPos.x - (int)iPos.x, mPos.y - (int)iPos.y);
	sf::Vector2i tilePos = sf::Vector2i((int)absPos.x / ((int)tileset->getTilePixelSize().x + tileset->getTilingPadding()), (int)absPos.y / ((int)tileset->getTilePixelSize().y + tileset->getTilingPadding()));
	int tileID = tilePos.x + tilePos.y * (int)tileset->getTilesetTileSize().x;
	selectedTile = tileset->getTile(tileID);
}