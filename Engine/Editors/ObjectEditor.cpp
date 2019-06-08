#include <ObjectEditor.h>



ObjectEditor::ObjectEditor(sf::RenderWindow* window, Map* map) {
	cout << "Press 'CTRL+O' or '2' to open or close Tilemap Editor! While it is opened you can press 'H' to hide or show object selector!" << endl;
	this->window = window;
	this->map = map;

	gui = new Gui(*window);
	
	objectSets = ComboBox::create();
	objectSets->connect("itemSelected", (std::function<void()>)std::bind(&ObjectEditor::itemSelectCallback, this));
	
	objectAssets = ComboBox::create();
	objectAssets->connect("itemSelected", (std::function<void()>)std::bind(&ObjectEditor::itemSelectCallback, this));
	objectAssets->setPosition(sf::Vector2f(0, objectSets->getSize().y));

	for (ObjectSet* objectSet : AssetStore::getAllObjectSets()) {
		objectSets->addItem(objectSet->getName());
	}
	
	gui->add(objectSets);
	gui->add(objectAssets);

	selectedObjectAssetRenderer = new sf::RectangleShape();
}

ObjectEditor::~ObjectEditor() {
	delete gui;
	delete selectedObjectAssetRenderer;
}

void ObjectEditor::Update(int updateElapsed, Camera* camera) {
	static bool keyRelaseFlag = false;
	if (((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) && sf::Keyboard::isKeyPressed(sf::Keyboard::O)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))) {
		if (keyRelaseFlag) {
			isActive = !isActive;
			selectedObjectAsset = nullptr;
			objectAssets->deselectItem();
			objectSets->deselectItem();
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
			objectSets->setVisible(!objectSets->isVisible());
			objectAssets->setVisible(!objectAssets->isVisible());
			keyRelaseFlag2 = false;
		}
	}
	else {
		keyRelaseFlag2 = true;
	}

}

void ObjectEditor::Render(Camera* camera) {
	if (!isActive)
		return;
	gui->draw();
	

	if (selectedObjectAsset != nullptr) {
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);
		sf::Vector2f cPos = camera->getPosition();
		sf::Vector2i tilePos = sf::Vector2i((int)mPos.x / (int)map->getTileset()->getTilePixelSize().x, (int)mPos.y / (int)map->getTileset()->getTilePixelSize().y);

		selectedObjectAssetRenderer->setTexture(selectedObjectAsset->getTile()->getTexture());
		selectedObjectAssetRenderer->setPosition(tilePos.x * map->getTileset()->getTilePixelSize().x - mathUtils::findMod(cPos.x, map->getTileset()->getTilePixelSize().x), tilePos.y * map->getTileset()->getTilePixelSize().y - mathUtils::findMod(cPos.y, map->getTileset()->getTilePixelSize().y));
		window->draw(*selectedObjectAssetRenderer);
	}
}

void ObjectEditor::HandleWindowEvent(sf::Event event, Camera* camera) {
	if (!isActive)
		return;

	gui->handleEvent(event);

	sf::Vector2i mPos = sf::Mouse::getPosition(*window);
	if (selectedObjectAsset == nullptr || (objectSets->isVisible() && mathUtils::intersectsWith(objectSets->getPosition(), objectSets->getSize(), mPos)) || (objectAssets->isVisible() && mathUtils::intersectsWith(objectAssets->getPosition(), objectAssets->getSize(), mPos)))
		return;
	
	static bool mouseRelaseFlag = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (mouseRelaseFlag) 
			mouseRelaseFlag = false;
	}
	else {
		if (!mouseRelaseFlag) {
			sf::Vector2f cPos = camera->getPosition();
			sf::Vector2i tilePos = sf::Vector2i((int)(cPos.x + mPos.x) / (int)map->getTileset()->getTilePixelSize().x, (int)(cPos.y + mPos.y) / (int)map->getTileset()->getTilePixelSize().y);
			map->AddGameObject(selectedObjectAsset, tilePos);
		}
		mouseRelaseFlag = true;
	}
}

void ObjectEditor::itemSelectCallback() {
	string selectedObjectSetItem = objectSets->getSelectedItem();
	
	if (!selectedObjectSetItem.empty() && lastSelectedObjectSet.compare(selectedObjectSetItem) != 0) {
		objectAssets->deselectItem();
		objectAssets->removeAllItems();
		lastSelectedObjectSet = selectedObjectSetItem;
		selectedObjectAsset = nullptr;
		delete selectedObjectAssetRenderer;
		selectedObjectAssetRenderer = new sf::RectangleShape();
		selectedObjectAssetRenderer->setSize(AssetStore::GetObjectSet(selectedObjectSetItem.c_str())->getTileset()->getTilePixelSize());
		for (ObjectAsset* objectAsset : AssetStore::GetObjectSet(selectedObjectSetItem.c_str())->getAllObjectAssets()) {
			objectAssets->addItem(objectAsset->getName());
		}
	}

	string selectedObjectAssetItem = objectAssets->getSelectedItem();
	if (!selectedObjectAssetItem.empty() && lastSelectedObjectAsset.compare(selectedObjectSetItem) != 0) {
		selectedObjectAsset = AssetStore::GetObjectSet(selectedObjectSetItem.c_str())->getObjectAsset(selectedObjectAssetItem);
		lastSelectedObjectAsset = selectedObjectAssetItem;
	}

}