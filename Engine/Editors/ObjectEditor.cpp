#include <ObjectEditor.h>



ObjectEditor::ObjectEditor(sf::RenderWindow* window, Map* map) {
	cout << "Press 'CTRL+O' or '2' to open or close Object Editor!" << endl;
	this->window = window;
	this->map = map;

	gui = new Gui(*window);
	child = ChildWindow::create("Object Editor");
	Label::Ptr selectObecjectSetLabel = Label::create("Please select a object set");
	Label::Ptr selectObjectAssetLabel = Label::create("Please select a object asset");

	child->add(selectObecjectSetLabel);

	int maxItems = 5;
	objectSets = ComboBox::create();
	objectSets->connect("itemSelected", (std::function<void()>)std::bind(&ObjectEditor::itemSelectCallback, this));
	objectSets->setPosition(0, selectObecjectSetLabel->getPosition().y + selectObecjectSetLabel->getSize().y);
	objectSets->setItemsToDisplay(maxItems);
	child->add(objectSets);

	selectObjectAssetLabel->setPosition(0, objectSets->getPosition().y + objectSets->getSize().y);
	child->add(selectObjectAssetLabel);

	objectAssets = ComboBox::create();
	objectAssets->connect("itemSelected", (std::function<void()>)std::bind(&ObjectEditor::itemSelectCallback, this));
	objectAssets->setPosition(sf::Vector2f(0, selectObjectAssetLabel->getPosition().y + selectObjectAssetLabel->getSize().y));
	objectAssets->setItemsToDisplay(maxItems);
	child->add(objectAssets);

	for (ObjectSet* objectSet : AssetStore::getAllObjectSets()) {
		objectSets->addItem(objectSet->getName());
	}

	float maxXSize = 0;
	for (Widget::Ptr p : child->getWidgets()) 
		if (p->getSize().x > maxXSize)
			maxXSize = p->getSize().x;

	child->setSize(maxXSize, objectAssets->getPosition().y + objectAssets->getSize().y);
	child->setTitleButtons(0);
	gui->add(child);

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

	if (gui->handleEvent(event))
		return;
	
	if (selectedObjectAsset == nullptr)
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