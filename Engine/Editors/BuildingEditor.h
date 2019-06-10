#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
class Camera;
#include <Camera.h>
#include <mathUtils.h>

using namespace std;
using namespace tgui;


class BuildingEditor {
public:
	BuildingEditor(sf::RenderWindow* window, Map* map);
	~BuildingEditor();

	void itemSelectCallback();
	void Render(Camera* camera);
	void Update(int updateElapsed, Camera* camera);
	void HandleWindowEvent(sf::Event event, Camera* camera);
	
private:
	sf::RenderWindow* window;
	Gui* gui;
	bool isActive = false;

	Map* map;

};