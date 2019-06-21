#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
class Camera;
#include <Camera.h>
#include <mathUtils.h>

using namespace std;
using namespace tgui;


class TilemapEditor {
public:
	TilemapEditor(sf::RenderWindow* window, Map* map);
	~TilemapEditor();

	void clickCallback();
	void Render(Camera* camera);
	void Update(int updateElapsed, Camera* camera);
	void HandleWindowEvent(sf::Event event, Camera* camera);
	

private:
	sf::RenderWindow* window;
	Gui* gui;
	ChildWindow::Ptr child;
	bool isActive = false;

	Map* map;
	Tile* selectedTile = nullptr;
	sf::RectangleShape* selectedTileRenderer;

	// GUI Widgets
	Picture::Ptr tilesetImage;


};