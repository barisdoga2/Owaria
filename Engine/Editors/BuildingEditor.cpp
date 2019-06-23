#include <BuildingEditor.h>



BuildingEditor::BuildingEditor(sf::RenderWindow* window, Map* map) {
	cout << "Press 'CTRL+B' or '3' to open or close Building Editor!" << endl;
	this->window = window;
	this->map = map;

	gui = new Gui(*window);
	child = ChildWindow::create("Building Editor");
	child->setTitleButtons(0);

	Label::Ptr selectBuildingLabel = Label::create("Please select a building asset");
	child->add(selectBuildingLabel);

	int maxItems = 5;
	buildings = ComboBox::create();
	buildings->connect("itemSelected", (std::function<void()>)std::bind(&BuildingEditor::itemSelectCallback, this));
	buildings->setMaximumItems(maxItems);
	buildings->setPosition(0, selectBuildingLabel->getSize().y);
	child->add(buildings);

	for (BuildingAsset* bA : AssetStore::getAllBuildingAssets()) 
		buildings->addItem(bA->getName());

	float maxXSize = 0;
	for (Widget::Ptr p : child->getWidgets()) {
		if (p->getSize().x > maxXSize)
			maxXSize = p->getSize().x;
	}

	child->setSize(maxXSize, buildings->getPosition().y + buildings->getSize().y);
	gui->add(child);

	tileRenderer = new sf::RectangleShape();
}

BuildingEditor::~BuildingEditor() {
	delete gui;
	delete tileRenderer;
}

void BuildingEditor::Update(int updateElapsed, Camera* camera) {
	static bool keyRelaseFlag = false;
	if (((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) && sf::Keyboard::isKeyPressed(sf::Keyboard::B)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))) {
		if (keyRelaseFlag) {
			isActive = !isActive;
			buildings->deselectItem();
			selectedBuildingAsset = nullptr;
			camera->SetFreeRoam(isActive);
			keyRelaseFlag = false;
		}
	}
	else {
		keyRelaseFlag = true;
	}

	if (!isActive)
		return;
}

void BuildingEditor::Render(Camera* camera) {
	if (!isActive)
		return;
	gui->draw();
	
	if (selectedBuildingAsset != nullptr) {
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);
		sf::Vector2f cPos = camera->getPosition();
		sf::Vector2i tilePos = sf::Vector2i((int)mPos.x / (int)map->getTileset()->getTilePixelSize().x, (int)mPos.y / (int)map->getTileset()->getTilePixelSize().y);

		for (int y = 0; y < selectedBuildingAsset->tilemapSize.y; y++) {
			for (int x = 0; x < selectedBuildingAsset->tilemapSize.x; x++) {
				Tile* t = (selectedBuildingAsset->mapGridTileIDs + x + y * selectedBuildingAsset->tilemapSize.x);
				tileRenderer->setTexture(t->getTexture());
				tileRenderer->setPosition((tilePos.x + x) * tileRenderer->getSize().x - mathUtils::findMod(cPos.x, selectedBuildingAsset->tileset->getTilePixelSize().x), (tilePos.y + y) * tileRenderer->getSize().y - mathUtils::findMod(cPos.y, selectedBuildingAsset->tileset->getTilePixelSize().y));
				window->draw(*tileRenderer);
			}
		}
	}
}

void BuildingEditor::HandleWindowEvent(sf::Event event, Camera* camera) {
	if (!isActive)
		return;

	if (gui->handleEvent(event))
		return;
	
	if (selectedBuildingAsset == nullptr)
		return;

	static bool mouseRelaseFlag = true;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (mouseRelaseFlag)
			mouseRelaseFlag = false;
	}
	else {
		if (!mouseRelaseFlag) {
			sf::Vector2i mPos = sf::Mouse::getPosition(*window);
			sf::Vector2f cPos = camera->getPosition();
			sf::Vector2i tilePos = sf::Vector2i((int)(cPos.x + mPos.x) / (int)map->getTileset()->getTilePixelSize().x, (int)(cPos.y + mPos.y) / (int)map->getTileset()->getTilePixelSize().y);
			map->AddBuilding(selectedBuildingAsset, tilePos);
			map->ApplyMarchingSquares();
		}
		mouseRelaseFlag = true;
	}
}

void BuildingEditor::itemSelectCallback() {
	selectedBuildingAsset = AssetStore::GetBuildingAsset(((string)buildings->getSelectedItem()).c_str());

	delete tileRenderer;
	tileRenderer = new sf::RectangleShape();
	tileRenderer->setSize(selectedBuildingAsset->tileset->getTilePixelSize());
}