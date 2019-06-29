#include <Inventory.h>



Inventory::Inventory(sf::RenderWindow* window, Player* player) {
	this->window = window;
	this->player = player;
	this->gui = new Gui(*window);

	textureLoader = Texture();
	textureLoader.load("../../Resources/Character.png");
	characterPicture = Picture::create(textureLoader);

	textureLoader.load("../../Resources/Inventory.png");
	inventoryPicture = Picture::create(textureLoader);

	textureLoader.load("../../Resources/Transparent.png");

	characterPicture->setPosition(SCREEN_WIDTH - characterPicture->getSize().x - (inventoryPicture->getSize().x - characterPicture->getSize().x) / 2 - 20, 100);
	characterPicture->connect("mousePressed", (std::function<void()>)std::bind(&Inventory::characterMousePressedCallback, this));
	characterPicture->connect("mouseReleased", (std::function<void()>)std::bind(&Inventory::characterMouseReleasedCallback, this));

	inventoryPicture->setPosition(SCREEN_WIDTH - inventoryPicture->getSize().x - 20, characterPicture->getPosition().y + characterPicture->getSize().y + 50);
	inventoryPicture->connect("mousePressed", (std::function<void()>)std::bind(&Inventory::inventoryMousePressedCallback, this));
	inventoryPicture->connect("mouseReleased", (std::function<void()>)std::bind(&Inventory::inventoryMouseReleasedCallback, this));

	gui->add(characterPicture);
	gui->add(inventoryPicture);

	Create();
}

Inventory::~Inventory() {

}


void Inventory::Update() {

}

void Inventory::Render() {
	gui->draw();

	if (inventoryMovingPicture) {
		sf::Texture* icon = AssetStore::GetItemAssetByID(inventoryMovingItemAssetID)->getIcon();
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(icon->getSize().x, icon->getSize().y));
		rect.setPosition(mPos.x - inventoryMovingPictureOffset.x, mPos.y - inventoryMovingPictureOffset.y);
		rect.setTexture(icon);

		window->draw(rect);
	}
	else if (characterMovingPicture) {
		sf::Texture* icon = AssetStore::GetItemAssetByID(characterMovingItemAssetID)->getIcon();
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(icon->getSize().x, icon->getSize().y));
		rect.setPosition(mPos.x - characterMovingPictureOffset.x, mPos.y - characterMovingPictureOffset.y);
		rect.setTexture(icon);

		window->draw(rect);
	}
}

int button;
void Inventory::HandleWindowEvent(sf::Event sevent) {

	button = sevent.mouseButton.button;

	if (sevent.mouseButton.button == sf::Mouse::Button::Right) {
		sevent.mouseButton.button = sf::Mouse::Button::Left;
	}

	if (!gui->handleEvent(sevent)) {
		if (sevent.type == sf::Event::MouseButtonReleased) {
			inventoryMovingPicture = false;
			characterMovingPicture = false;
		}
	}
}

void Inventory::characterMousePressedCallback() {

}

void Inventory::characterMouseReleasedCallback() {
	if (button == sf::Mouse::Button::Right)
		return;

	inventoryMovingPicture = false;
	characterMovingPicture = false;
}

void Inventory::inventoryMousePressedCallback() {

}

void Inventory::inventoryMouseReleasedCallback() {
	if (button == sf::Mouse::Button::Right)
		return;

	inventoryMovingPicture = false;
	characterMovingPicture = false;
}

void Inventory::pictureMouseClickedCallback(int x, int y) {
	if (button == sf::Mouse::Button::Left)
		return;

	int id = player->inventory[x + y * inventorySize.x];
	if (id != 0) {
		ItemAsset* itemAsset = AssetStore::GetItemAssetByID(id);
		int type = itemAsset->getType();
		if (type >= WEARABLE_MINIMUM && type < WEARABLE_COUNT) {
			
			// Already wearing something
			if (player->items[type] != nullptr) {
				ItemAsset* weared = player->items[type]->GetItemAsset();
				bool retVal = player->WearItem(itemAsset);
				if (retVal) {
					player->inventory[x + y * inventorySize.x] = weared->getID();
				}
			}
			// Not wearing something
			else {
				bool retVal = player->WearItem(itemAsset);
				if (retVal) {
					player->inventory[x + y * inventorySize.x] = 0;
				}
			}

			Create();
		}
	}
}

void Inventory::pictureMousePressedCallback(int x, int y) {
	if (button == sf::Mouse::Button::Right)
		return;

	if (player->inventory[x + y * inventorySize.x] == 0)
		return;

	Picture::Ptr pic = inventoryPictures[x + y * inventorySize.x];
	sf::Vector2i mPos = sf::Mouse::getPosition(*window);

	inventoryMovingPicture = true;
	inventoryMovingItemAssetID = player->inventory[x + y * inventorySize.x];
	inventoryMovingPictureOffset = sf::Vector2i(mPos.x - pic->getAbsolutePosition().x, mPos.y - pic->getAbsolutePosition().y);
	inventoryMovingPictureIndices.x = x;
	inventoryMovingPictureIndices.y = y;
}

void Inventory::pictureMouseReleasedCallback(int x, int y) {
	if (button == sf::Mouse::Button::Right)
		return;

	if (inventoryMovingPicture) {
		inventoryMovingPicture = false;

		int tmp = player->inventory[inventoryMovingPictureIndices.x + inventoryMovingPictureIndices.y * inventorySize.x];
		player->inventory[inventoryMovingPictureIndices.x + inventoryMovingPictureIndices.y * inventorySize.x] = player->inventory[x + y * inventorySize.x];
		player->inventory[x + y * inventorySize.x] = tmp;
		
		Create();
	}
	else if (characterMovingPicture) {
		characterMovingPicture = false;

		int id = player->inventory[x + y * inventorySize.x];
		if (id == 0) {
			player->UnWearItem(AssetStore::GetItemAssetByID(characterMovingItemAssetID));
			player->inventory[x + y * inventorySize.x] = characterMovingItemAssetID;
		}
		else {
			ItemAsset* moving = AssetStore::GetItemAssetByID(characterMovingItemAssetID);
			ItemAsset* to = AssetStore::GetItemAssetByID(id);
			if (moving->getType() == to->getType()) {
				player->UnWearItem(moving);
				player->WearItem(to);
				player->inventory[x + y * inventorySize.x] = moving->getID();
			}
		}

		Create();
	}
}

void Inventory::characterPictureMouseClickedCallback(int TYPE) {
	if (button == sf::Mouse::Button::Left)
		return;

	Item* item = player->items[TYPE];

	if (item != nullptr) {
		// Find empty cell
		int i = 0;
		bool found = false;
		for (i = 0; i < inventorySize.x * inventorySize.y; i++) {
			if (player->inventory[i] == 0) {
				found = true;
				break;
			}
		}

		player->UnWearItem(item->GetItemAsset());
		player->inventory[i] = item->GetItemAsset()->getID();
		
		Create();
	}
}

void Inventory::characterPictureMousePressedCallback(int TYPE) {
	if (button == sf::Mouse::Button::Right)
		return;

	if (player->items[TYPE] != nullptr) {
		Picture::Ptr pic = itemPictures[TYPE];
		sf::Vector2i mPos = sf::Mouse::getPosition(*window);

		characterMovingPicture = true;
		characterMovingItemAssetID = player->items[TYPE]->GetItemAsset()->getID();
		characterMovingPictureOffset = sf::Vector2i(mPos.x - pic->getAbsolutePosition().x, mPos.y - pic->getAbsolutePosition().y);
	}
}

void Inventory::characterPictureReleasedCallback(int TYPE) {
	if (button == sf::Mouse::Button::Right)
		return;

	if (inventoryMovingPicture) {
		if (AssetStore::GetItemAssetByID(inventoryMovingItemAssetID)->getType() == TYPE) {
			
			if (player->items[TYPE] != nullptr) {
				int tmpID = player->items[TYPE]->GetItemAsset()->getID();

				bool retVal = player->WearItem(AssetStore::GetItemAssetByID(inventoryMovingItemAssetID));
				if (retVal)
					player->inventory[inventoryMovingPictureIndices.x + inventoryMovingPictureIndices.y * inventorySize.x] = tmpID;				
			}
			else {
				bool retVal = player->WearItem(AssetStore::GetItemAssetByID(inventoryMovingItemAssetID));
				if (retVal)
					player->inventory[inventoryMovingPictureIndices.x + inventoryMovingPictureIndices.y * inventorySize.x] = 0;
			}

			Create();
		}

		inventoryMovingPicture = false;
	}
}

void Inventory::Create() {
	for (Picture::Ptr p : inventoryPictures)
		gui->remove(p);

	for (int i = 0; i < WEARABLE_COUNT; i++)
		gui->remove(itemPictures[i]);

	gui->remove(coin);

	for (int i = 0; i < WEARABLE_COUNT; i++) {
		Item* item = player->items[i];

		if(item != nullptr)
			itemPictures[i] = Picture::create(*item->GetItemAsset()->getIcon());
		else
			itemPictures[i] = Picture::create(textureLoader);

		itemPictures[i]->setPosition(characterPicture->getPosition().x + itemPositions[i].x, characterPicture->getPosition().y + itemPositions[i].y);
		itemPictures[i]->connect("mousePressed", (std::function<void()>)std::bind(&Inventory::characterPictureMousePressedCallback, this, i));
		itemPictures[i]->connect("mouseReleased", (std::function<void()>)std::bind(&Inventory::characterPictureReleasedCallback, this, i));
		itemPictures[i]->connect("clicked", (std::function<void()>)std::bind(&Inventory::characterPictureMouseClickedCallback, this, i));
		gui->add(itemPictures[i]);
	}

	for (int y = 0; y < inventorySize.y; y++) {
		int yy = (int)inventoryPicture->getPosition().y + inventoryCellStart.y + y * (inventoryCellPadding + inventoryCellSize.y);

		for (int x = 0; x < inventorySize.x; x++) {
			int xx = (int)inventoryPicture->getPosition().x + inventoryCellStart.x + x * (inventoryCellPadding + inventoryCellSize.x);
			int index = y * inventorySize.x + x;
			int itemAssetId = player->inventory[index];
			if (itemAssetId == 0)
				inventoryPictures[index] = Picture::create(textureLoader);
			else
				inventoryPictures[index] = Picture::create(*AssetStore::GetItemAssetByID(itemAssetId)->getIcon(), false);

			inventoryPictures[index]->setPosition(xx, yy);
			inventoryPictures[index]->connect("mousePressed", (std::function<void()>)std::bind(&Inventory::pictureMousePressedCallback, this, x, y));
			inventoryPictures[index]->connect("mouseReleased", (std::function<void()>)std::bind(&Inventory::pictureMouseReleasedCallback, this, x, y));
			inventoryPictures[index]->connect("clicked", (std::function<void()>)std::bind(&Inventory::pictureMouseClickedCallback, this, x, y));
			gui->add(inventoryPictures[index]);
		}
	}

	coin = Label::create("100000");
	coin->setTextSize(9);
	coin->getRenderer()->setTextColor(Color::Blue);
	coin->setPosition(inventoryPicture->getPosition().x + coinPosition.x, inventoryPicture->getPosition().y + coinPosition.y);

	gui->add(coin);
}