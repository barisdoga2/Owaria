#include <Inventory.h>



Inventory::Inventory(sf::RenderWindow* window, Player* player) {
	this->window = window;
	this->player = player;
	this->gui = new Gui(*window);
	
	Texture t = Texture();
	t.load("../../Resources/Character.png");
	characterPicture = Picture::create(t);

	t.load("../../Resources/Inventory.png");
	inventoryPicture = Picture::create(t);

	characterPicture->setPosition(SCREEN_WIDTH - characterPicture->getSize().x - (inventoryPicture->getSize().x - characterPicture->getSize().x) / 2 - 20, 100);
	inventoryPicture->setPosition(SCREEN_WIDTH - inventoryPicture->getSize().x - 20, characterPicture->getPosition().y + characterPicture->getSize().y + 50);

	gui->add(characterPicture);
	gui->add(inventoryPicture);

	for (int y = 0; y < inventorySize.y; y++) {
		int yy = inventoryPicture->getPosition().y + inventoryCellStart.y + y * (inventoryCellPadding + inventoryCellSize.y);

		for (int x = 0; x < inventorySize.x; x++) {
			int xx = inventoryPicture->getPosition().x + inventoryCellStart.x + x * (inventoryCellPadding + inventoryCellSize.x);
			int index = y * (int)inventorySize.x + x;
			int itemAssetId = player->inventory[index];
			if (itemAssetId == 0)
				pictures[index] = Picture::create();
			else
				pictures[index] = Picture::create(*AssetStore::GetItemAssetByID(itemAssetId)->getIcon(), false);
			
			pictures[index]->setPosition(xx, yy);
			//pictures[index]->connect("clicked", (std::function<void()>)std::bind(&Inventory::clikCallback, this, x, y));
			gui->add(pictures[index]);
		}
	}

	Label::Ptr coin = Label::create("100000");
	coin->setTextSize(9);
	coin->getRenderer()->setTextColor(Color::Blue);
	coin->setPosition(inventoryPicture->getPosition().x + coinPosition.x, inventoryPicture->getPosition().y + coinPosition.y);

	gui->add(coin);
}

Inventory::~Inventory() {

}


void Inventory::Update() {

}

void Inventory::Render() {
	gui->draw();
}

void Inventory::HandleWindowEvent(sf::Event event) {
	gui->handleEvent(event);

}

void Inventory::clikCallback(int x, int y) {
	cout << x << "|" << y << endl;
}