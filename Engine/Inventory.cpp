#include <Inventory.h>



Inventory::Inventory(sf::RenderWindow* window, Player* player) {
	g = new Gui(*window);
	
	Texture t = Texture();
	t.load("../../Resources/Character.png");
	Picture::Ptr picture = Picture::create(t);

	t.load("../../Resources/Inventory.png");
	Picture::Ptr picture2 = Picture::create(t);

	picture->setPosition(SCREEN_WIDTH - picture->getSize().x - (picture2->getSize().x - picture->getSize().x) / 2 - 20, 100);
	picture2->setPosition(SCREEN_WIDTH - picture2->getSize().x - 20, picture->getPosition().y + picture->getSize().y + 50);

	g->add(picture);
	g->add(picture2);

	for (int y = 0; y < inventorySize.y; y++) {
		int yy = picture2->getPosition().y + inventoryCellStart.y + y * (inventoryCellPadding + inventoryCellSize.y);

		for (int x = 0; x < inventorySize.x; x++) {
			int xx = picture2->getPosition().x + inventoryCellStart.x + x * (inventoryCellPadding + inventoryCellSize.x);
			int index = y * (int)inventorySize.x + x;
			int itemAssetId = player->inventory[index];
			if (itemAssetId == 0)
				pictures[index] = Picture::create();
			else
				pictures[index] = Picture::create(*AssetStore::GetItemAssetByID(itemAssetId)->getIcon(), false);
			
			pictures[index]->setPosition(xx, yy);
			
			g->add(pictures[index]);
			
			
		}
	}

	Label::Ptr coin = Label::create("100000");
	coin->setTextSize(9);
	coin->getRenderer()->setTextColor(Color::Blue);
	coin->setPosition(picture2->getPosition().x + coinPosition.x, picture2->getPosition().y + coinPosition.y);

	g->add(coin);
}

Inventory::~Inventory() {

}


void Inventory::Update() {

}

void Inventory::Render(sf::RenderWindow* window) {
	g->draw();
}

void Inventory::HandleWindowEvent(sf::Event event) {

}