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

	void Update();
	void Render();
	void HandleWindowEvent(sf::Event event);

	const sf::Vector2i inventorySize = sf::Vector2i(5, 10);


private:
	const int inventoryCellPadding = 11;
	const sf::Vector2i inventoryCellStart = sf::Vector2i(12, 273);
	const sf::Vector2i inventoryCellSize = sf::Vector2i(34, 35);
	const sf::Vector2i coinPosition = sf::Vector2i(115, 734);

	Gui * gui;
	sf::RenderWindow* window;
	Player* player;
	Texture textureLoader;

	Picture::Ptr inventoryPicture;
	void inventoryMouseReleasedCallback();

	void pictureMouseClickedCallback(int x, int y, bool isCharacter, int WEARABLE_TYPE);
	void pictureMousePressedCallback(int x, int y, bool isCharacter, int WEARABLE_TYPE);
	void pictureMouseReleasedCallback(int x, int y, bool isCharacter, int WEARABLE_TYPE);

	Label::Ptr coin;
	Picture::Ptr inventoryPictures[50];

	sf::Vector2i inventoryMovingPictureIndices;
	sf::Vector2i inventoryMovingPictureOffset;
	int inventoryMovingItemAssetID;
	bool inventoryMovingPicture = false;

	sf::Vector2i characterMovingPictureOffset;
	int characterMovingItemAssetID;
	bool characterMovingPicture = false;

	void Create();

	sf::Vector2i itemPositions[WEARABLE_COUNT] = {
		sf::Vector2i(99, 142),	// WEARABLE_WEAPON
		sf::Vector2i(194, 142),	// WEARABLE_SHIELD
		sf::Vector2i(146, 59),	// WEARABLE_HELMET
		sf::Vector2i(99, 101),	// WEARABLE_PAULDRON
		sf::Vector2i(194, 101),	// WEARABLE_GAUNTLETS
		sf::Vector2i(146, 182)	// WEARABLE_BOOTS
	};
	Picture::Ptr itemPictures[WEARABLE_COUNT];
};