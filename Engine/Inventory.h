#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <Settings.h>
#include <Player.h>
#include <AssetStore.h>

using namespace tgui;

#define START_X 15
#define START_Y 15
#define SIZE_X 47
#define SIZE_Y 47
#define PADDING 14

#define COUNT_COLUMNS 5
#define COUNT_ROWS 4

class Inventory {

public:
	Inventory(sf::RenderWindow* window, Player* player);
	~Inventory();

	void Update();
	void Render(sf::RenderWindow* window);
	void HandleWindowEvent(sf::Event event);

	const sf::Vector2f inventorySize = sf::Vector2f(5, 10);


private:
	Gui * g;

	const int inventoryCellPadding = 11;
	const sf::Vector2f inventoryCellStart = sf::Vector2f(12, 12);
	const sf::Vector2f inventoryCellSize = sf::Vector2f(34, 35);

	const sf::Vector2f coinPosition = sf::Vector2f(115, 475);
	
	Picture::Ptr pictures[50];
};