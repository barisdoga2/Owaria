#include <Inventory.h>



Inventory::Inventory(sf::RenderWindow* window, Player* player) {
	cout << "Press I to open or close Inventory." << endl;
	this->window = window;
	this->player = player;
	this->gui = new Gui(*window);

	textureLoader = Texture();

	textureLoader.load("../../Resources/Inventory.png");
	inventoryPicture = Picture::create(textureLoader);

	textureLoader.load("../../Resources/Transparent.png");

	inventoryPicture->setPosition(SCREEN_WIDTH - inventoryPicture->getSize().x - 20, (SCREEN_HEIGHT - (int)inventoryPicture->getSize().y) / 2);
	inventoryPicture->connect("mousePressed", (std::function<void()>)std::bind(&Inventory::inventoryMousePressed, this));
	inventoryPicture->connect("mouseReleased", (std::function<void()>)std::bind(&Inventory::inventoryMouseReleased, this));

	gui->add(inventoryPicture);

	CreateWidgets();
}

Inventory::~Inventory() {
	for (int i = 0; i < WEARABLE_COUNT; i++)
		if (GetWear(i) != nullptr)
			delete GetWear(i);
	
	delete gui;
}

void Inventory::Render() {
	if (!visible)
		return;

	gui->draw();

	if (cellMoving) {
		sf::Texture* icon = cellMovingAsset->getIcon();
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f((float)icon->getSize().x, (float)icon->getSize().y));
		rect.setPosition((float)(mPos.x - cellMovingOffset.x), (float)(mPos.y - cellMovingOffset.y));
		rect.setTexture(icon);

		window->draw(rect);
	}
	else if (wearableMoving) {
		sf::Texture* icon = wearableMovingAsset->getIcon();
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f((float)icon->getSize().x, (float)icon->getSize().y));
		rect.setPosition((float)(mPos.x - wearableMovingOffset.x), (float)(mPos.y - wearableMovingOffset.y));
		rect.setTexture(icon);

		window->draw(rect);
	}

	if (inventoryDragging) {
		sf::Vector2i current = sf::Mouse::getPosition(*window);
		sf::Vector2i diff = sf::Vector2i(current.x - inventoryDraggingStart.x, current.y - inventoryDraggingStart.y);
		inventoryDraggingStart = current;

		for (Widget::Ptr p : gui->getWidgets()) {
			p->setPosition(p->getPosition().x + diff.x, p->getPosition().y + diff.y);
		}
	}
}

/*****************************************************************************************************************************************************************/
/*************************************************************************************HELPER FUNCTIONS************************************************************/
/*****************************************************************************************************************************************************************/

void Inventory::WearItem(sf::Vector2i inventoryPosition, int TO_TYPE) {
	ItemAsset* asset = GetInventoryCell(inventoryPosition);
	if (asset->getType() != TO_TYPE)
		return;

	Item* wear = GetWear(asset->getType());

	inventoryItemIDs[inventoryPosition.x + inventoryPosition.y * INVENTORY_SIZE.x] = (wear == nullptr ? 0 : wear->GetItemAsset()->getID());

	switch (asset->getType())
	{
	case WEARABLE_WEAPON:
	case WEARABLE_SHIELD:
		wearableItems[asset->getType()] = new MeleeWeapon(asset, player->GetAnimation(asset->getAnimationAsset()->getName()));
		break;

	default:
		wearableItems[asset->getType()] = new Armor(asset);
		break;
	}
}


void Inventory::UnWearItem(int WEARABLE_TYPE, sf::Vector2i toCell) {
	int id = inventoryItemIDs[toCell.x + toCell.y * INVENTORY_SIZE.x];

	if (id == 0) {
		inventoryItemIDs[toCell.x + toCell.y * INVENTORY_SIZE.x] = GetWear(WEARABLE_TYPE)->GetItemAsset()->getID();
		wearableItems[WEARABLE_TYPE] = nullptr;
	}
}

void Inventory::UnWearItem(int WEARABLE_TYPE) {
	int i, found = false;
	for (i = 0; i < INVENTORY_SIZE.x * INVENTORY_SIZE.y; i++)
		if (inventoryItemIDs[i] == 0) {
			found = true;
			break;
		}

	if (found) {
		inventoryItemIDs[i] = GetWear(WEARABLE_TYPE)->GetItemAsset()->getID();
		wearableItems[WEARABLE_TYPE] = nullptr;
	}
}

Item* Inventory::GetWear(int WEARABLE_TYPE) {
	return wearableItems[WEARABLE_TYPE];
}

ItemAsset* Inventory::GetInventoryCell(sf::Vector2i inventoryPosition) {
	return AssetStore::GetItemAssetByID(inventoryItemIDs[inventoryPosition.x + inventoryPosition.y * INVENTORY_SIZE.x]);
}

void Inventory::SwapInventoryCells(sf::Vector2i cellInventoryPosition1, sf::Vector2i cellInventoryPosition2) {
	int tmpID = inventoryItemIDs[cellInventoryPosition1.x + cellInventoryPosition1.y * INVENTORY_SIZE.x];
	inventoryItemIDs[cellInventoryPosition1.x + cellInventoryPosition1.y * INVENTORY_SIZE.x] = inventoryItemIDs[cellInventoryPosition2.x + cellInventoryPosition2.y * INVENTORY_SIZE.x];
	inventoryItemIDs[cellInventoryPosition2.x + cellInventoryPosition2.y * INVENTORY_SIZE.x] = tmpID;
}
/*****************************************************************************************************************************************************************/
/*****************************************************************************************************************************************************************/
/*****************************************************************************************************************************************************************/

void Inventory::HandleWindowEvent(sf::Event sevent) {
	if (!visible)
		return;

	// Tgui doesnt support right click events, so this is a hack to pass right click events through tgui
	// Handle button with "lastSFButton" variable on callbacks
	lastSFButton = sevent.mouseButton.button;
	if (sevent.mouseButton.button == sf::Mouse::Button::Right) {
		sevent.mouseButton.button = sf::Mouse::Button::Left;
	}

	if (!gui->handleEvent(sevent)) {
		// Cancel Drag Event if event not handled in tgui
		if (sevent.type == sf::Event::MouseButtonReleased) {
			cellMoving = false;
			wearableMoving = false;
		}
	}
}

// Inventory Picture Drag Cancel Event
void Inventory::inventoryMouseReleased() {
	if (lastSFButton == sf::Mouse::Button::Left) {
		cellMoving = false;
		wearableMoving = false;
	}

	inventoryDragging = false;
}

// Activate Inventory Dragging
void Inventory::inventoryMousePressed() {
	inventoryDragging = true;
	inventoryDraggingStart = sf::Mouse::getPosition(*window);
}

// Trigger Item Right Click Event
void Inventory::cellMouseClicked(sf::Vector2i inventoryCellPosition, bool isCharacter, int WEARABLE_TYPE) {

	cellMoving = false;
	wearableMoving = false;

	// Wearable Right Click
	if (isCharacter && lastSFButton == sf::Mouse::Button::Right && GetWear(WEARABLE_TYPE) != nullptr) {
		UnWearItem(WEARABLE_TYPE);

		CreateWidgets();
	}
	// Inventory Right Click
	else if (lastSFButton == sf::Mouse::Button::Right && GetInventoryCell(inventoryCellPosition) != nullptr) {
		WearItem(inventoryCellPosition, GetInventoryCell(inventoryCellPosition)->getType());

		CreateWidgets();
	}

}

// Trigger Item Drag Event
void Inventory::cellMousePressed(sf::Vector2i inventoryCellPosition, bool isCharacter, int WEARABLE_TYPE) {

	// Wearable Drag Trigger
	if (isCharacter && lastSFButton == sf::Mouse::Button::Left && GetWear(WEARABLE_TYPE) != nullptr) {
		Picture::Ptr pic = wearableCells[WEARABLE_TYPE];
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);

		wearableMoving = true;
		wearableMovingAsset = GetWear(WEARABLE_TYPE)->GetItemAsset();
		wearableMovingOffset = sf::Vector2i(mPos.x - (int)pic->getAbsolutePosition().x, mPos.y - (int)pic->getAbsolutePosition().y);
	}
	// Inventory Drag Trigger
	else if (lastSFButton == sf::Mouse::Button::Left && GetInventoryCell(inventoryCellPosition) != nullptr) {
		Picture::Ptr pic = inventoryCells[inventoryCellPosition.x + inventoryCellPosition.y * INVENTORY_SIZE.x];
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);

		cellMoving = true;
		cellMovingAsset = GetInventoryCell(inventoryCellPosition);
		cellMovingOffset = sf::Vector2i(mPos.x - (int)pic->getAbsolutePosition().x, mPos.y - (int)pic->getAbsolutePosition().y);
		cellMovingIndice = inventoryCellPosition;
	}
}

// Trigger Item Drag Finished
void Inventory::cellMouseReleased(sf::Vector2i inventoryCellPosition, bool isCharacter, int WEARABLE_TYPE) {

	// Inventory Item Dragged to Wearable Slot
	if (isCharacter && lastSFButton == sf::Mouse::Button::Left && cellMoving) {
		if (cellMovingAsset->getType() == WEARABLE_TYPE) {
			WearItem(cellMovingIndice, WEARABLE_TYPE);

			CreateWidgets();
		}

		cellMoving = false;
	}
	else if (!isCharacter && lastSFButton == sf::Mouse::Button::Left) {
		// Inventory Slot Dragged to Inventory Slot
		if (cellMoving) {
			cellMoving = false;

			SwapInventoryCells(cellMovingIndice, inventoryCellPosition);

			CreateWidgets();
		}
		// Wearable Slot Dragged to Inventory
		else if (wearableMoving) {
			wearableMoving = false;

			// If Dragged Inventory Slot is Empty
			if (GetInventoryCell(inventoryCellPosition) == nullptr) {
				UnWearItem(wearableMovingAsset->getType(), inventoryCellPosition);
			}
			else {
				WearItem(inventoryCellPosition, wearableMovingAsset->getType());
			}

			CreateWidgets();
		}
	}
	else {
		cellMoving = false;
		wearableMoving = false;
	}
}


void Inventory::CreateWidgets() {
	// Clean All Widgets
	for (Picture::Ptr p : inventoryCells)
		gui->remove(p);

	for (int i = 0; i < WEARABLE_COUNT; i++)
		gui->remove(wearableCells[i]);

	gui->remove(inventoryCoin);

	// Create Wearable Picture Widgets
	for (int i = 0; i < WEARABLE_COUNT; i++) {
		Item* item = GetWear(i);

		if (item != nullptr)
			wearableCells[i] = Picture::create(*item->GetItemAsset()->getIcon());
		else
			wearableCells[i] = Picture::create(textureLoader);

		wearableCells[i]->setPosition(inventoryPicture->getPosition().x + WEARABLE_POSITIONS[i].x, inventoryPicture->getPosition().y + WEARABLE_POSITIONS[i].y);
		wearableCells[i]->connect("mousePressed", (std::function<void()>)std::bind(&Inventory::cellMousePressed, this, sf::Vector2i(), true, i));
		wearableCells[i]->connect("mouseReleased", (std::function<void()>)std::bind(&Inventory::cellMouseReleased, this, sf::Vector2i(), true, i));
		wearableCells[i]->connect("clicked", (std::function<void()>)std::bind(&Inventory::cellMouseClicked, this, sf::Vector2i(), true, i));
		gui->add(wearableCells[i]);
	}

	// Create Inventory Picture Widgets
	for (int y = 0; y < INVENTORY_SIZE.y; y++) {
		int yy = (int)inventoryPicture->getPosition().y + INVENTORY_CELL_START.y + y * (INVENTORY_CELL_PADDING.y + INVENTORY_CELL_SIZE.y);

		for (int x = 0; x < INVENTORY_SIZE.x; x++) {
			int xx = (int)inventoryPicture->getPosition().x + INVENTORY_CELL_START.x + x * (INVENTORY_CELL_PADDING.x + INVENTORY_CELL_SIZE.x);
			int index = y * INVENTORY_SIZE.x + x;

			ItemAsset* itemAsset = GetInventoryCell(sf::Vector2i(x, y));
			if (itemAsset == nullptr)
				inventoryCells[index] = Picture::create(textureLoader);
			else
				inventoryCells[index] = Picture::create(*itemAsset->getIcon(), false);

			inventoryCells[index]->setPosition(xx, yy);
			inventoryCells[index]->connect("mousePressed", (std::function<void()>)std::bind(&Inventory::cellMousePressed, this, sf::Vector2i(x, y), false, 0));
			inventoryCells[index]->connect("mouseReleased", (std::function<void()>)std::bind(&Inventory::cellMouseReleased, this, sf::Vector2i(x, y), false, 0));
			inventoryCells[index]->connect("clicked", (std::function<void()>)std::bind(&Inventory::cellMouseClicked, this, sf::Vector2i(x, y), false, 0));
			gui->add(inventoryCells[index]);
		}
	}

	// Create Coin Widgets
	inventoryCoin = Label::create("100000");
	inventoryCoin->setTextSize(9);
	inventoryCoin->getRenderer()->setTextColor(Color::Blue);
	inventoryCoin->setPosition(inventoryPicture->getPosition().x + INVENTORY_COIN_POSITION.x, inventoryPicture->getPosition().y + INVENTORY_COIN_POSITION.y);

	gui->add(inventoryCoin);
}

void Inventory::setVisible(bool flag) {
	visible = flag;
}

bool Inventory::isVisible() {
	return visible;
}