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

	const sf::Vector2f inventorySize = sf::Vector2f(5, 10);


private:
	Gui * gui;
	sf::RenderWindow* window;
	Player* player;
	Picture::Ptr characterPicture;
	Picture::Ptr inventoryPicture;

	const int inventoryCellPadding = 11;
	const sf::Vector2f inventoryCellStart = sf::Vector2f(12, 12);
	const sf::Vector2f inventoryCellSize = sf::Vector2f(34, 35);

	const sf::Vector2f coinPosition = sf::Vector2f(115, 475);

	const sf::Vector2f helmetPosition = sf::Vector2f(60, 12);
	const sf::Vector2f pauldronPosition = sf::Vector2f(13, 54);
	const sf::Vector2f weaponPosition = sf::Vector2f(13, 95);
	const sf::Vector2f bootsPosition = sf::Vector2f(61, 135);
	const sf::Vector2f shieldPosition = sf::Vector2f(108, 95);
	const sf::Vector2f gountletPosition = sf::Vector2f(108, 54);
	
	Picture::Ptr pictures[50];

	void clikCallback(int x, int y);
};