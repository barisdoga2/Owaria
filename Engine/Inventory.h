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
	const sf::Vector2i inventoryCellStart = sf::Vector2i(12, 12);
	const sf::Vector2i inventoryCellSize = sf::Vector2i(34, 35);
	const sf::Vector2i coinPosition = sf::Vector2i(115, 475);

	Gui * gui;
	sf::RenderWindow* window;
	Player* player;
	Texture textureLoader;

	Picture::Ptr characterPicture;
	Picture::Ptr inventoryPicture;
	void characterMousePressedCallback();
	void characterMouseReleasedCallback();
	void inventoryMousePressedCallback();
	void inventoryMouseReleasedCallback();

	sf::Vector2i inventoryMovingPictureIndices;
	sf::Vector2i inventoryMovingPictureOffset;
	int inventoryMovingItemAssetID;
	bool inventoryMovingPicture = false;
	void pictureMouseClickedCallback(int x, int y);
	void pictureMousePressedCallback(int x, int y);
	void pictureMouseReleasedCallback(int x, int y);


	sf::Vector2i characterMovingPictureOffset;
	int characterMovingItemAssetID;
	bool characterMovingPicture = false;
	void characterPictureMouseClickedCallback(int type);
	void characterPictureMousePressedCallback(int type);
	void characterPictureReleasedCallback(int type);

	Label::Ptr coin;
	Picture::Ptr inventoryPictures[50];

	void Create();

	sf::Vector2i itemPositions[WEARABLE_COUNT] = {
		sf::Vector2i(13, 95),	// WEARABLE_WEAPON
		sf::Vector2i(108, 95),	// WEARABLE_SHIELD
		sf::Vector2i(60, 12),	// WEARABLE_HELMET
		sf::Vector2i(13, 54),	// WEARABLE_PAULDRON
		sf::Vector2i(108, 54),	// WEARABLE_GAUNTLETS
		sf::Vector2i(61, 135)	// WEARABLE_BOOTS
	};
	Picture::Ptr itemPictures[WEARABLE_COUNT];
};