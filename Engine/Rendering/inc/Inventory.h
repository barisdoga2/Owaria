#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <Settings.h>
#include <Player.h>
#include <AssetStore.h>

using namespace tgui;



class Inventory {

public:
	Inventory(sf::RenderWindow* window, Player* player);
	~Inventory();

	void Render();
	void HandleWindowEvent(sf::Event event);
	bool isVisible();
	void setVisible(bool flag);

	void WearItem(sf::Vector2i inventoryPosition);
	void UnWearItem(int WEARABLE_TYPE);
	Item* GetWear(int WEARABLE_TYPE);
	ItemAsset* GetInventoryCell(sf::Vector2i inventoryPosition);
	void SwapInventoryCells(sf::Vector2i cellInventoryPosition1, sf::Vector2i cellInventoryPosition2);

private:
	// Player
	Item * wearableItems[WEARABLE_COUNT] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	int inventoryItemIDs[5 * 10] = {
		1,	2,	0,	0,	0,
		11,	12,	13,	14,	0,
		20,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0
	};

	// GUI
	const sf::Vector2i INVENTORY_SIZE					  =		sf::Vector2i(5, 10);
	const sf::Vector2i INVENTORY_CELL_PADDING			  =		sf::Vector2i(11, 11);
	const sf::Vector2i INVENTORY_CELL_START				  =		sf::Vector2i(12, 273);
	const sf::Vector2i INVENTORY_CELL_SIZE				  =		sf::Vector2i(34, 35);
	const sf::Vector2i INVENTORY_COIN_POSITION			  =		sf::Vector2i(115, 734);
	const sf::Vector2i WEARABLE_POSITIONS[WEARABLE_COUNT] = {
																sf::Vector2i(99, 142),	// WEARABLE_WEAPON
																sf::Vector2i(194, 142),	// WEARABLE_SHIELD
																sf::Vector2i(146, 59),	// WEARABLE_HELMET
																sf::Vector2i(99, 101),	// WEARABLE_PAULDRON
																sf::Vector2i(194, 101),	// WEARABLE_GAUNTLETS
																sf::Vector2i(146, 182)	// WEARABLE_BOOTS
															};

	Gui * gui;
	sf::RenderWindow* window;
	Player* player;
	Texture textureLoader;
	bool visible = false;

	Picture::Ptr inventoryPicture;
	void inventoryMousePressed();
	void inventoryMouseReleased();
	bool inventoryDragging = false;
	sf::Vector2i inventoryDraggingStart;

	Label::Ptr inventoryCoin;
	Picture::Ptr inventoryCells[50];
	Picture::Ptr wearableCells[WEARABLE_COUNT];
	void cellMouseClicked(sf::Vector2i inventoryCellPosition, bool isCharacter, int WEARABLE_TYPE);
	void cellMousePressed(sf::Vector2i inventoryCellPosition, bool isCharacter, int WEARABLE_TYPE);
	void cellMouseReleased(sf::Vector2i inventoryCellPosition, bool isCharacter, int WEARABLE_TYPE);

	sf::Vector2i cellMovingIndice;
	sf::Vector2i cellMovingOffset;
	ItemAsset* cellMovingAsset;
	bool cellMoving = false;

	sf::Vector2i wearableMovingOffset;
	ItemAsset* wearableMovingAsset;
	bool wearableMoving = false;

	void CreateWidgets();
	
	int lastSFButton;

};