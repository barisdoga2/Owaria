#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
class Camera;
#include <Camera.h>
#include <mathUtils.h>

using namespace std;
using namespace tgui;


class ObjectEditor {
public:
	ObjectEditor(sf::RenderWindow* window, Map* map);
	~ObjectEditor();

	void itemSelectCallback();
	void Render(Camera* camera);
	void Update(int updateElapsed, Camera* camera);
	void HandleWindowEvent(sf::Event event, Camera* camera);
	
private:
	sf::RenderWindow* window;
	Gui* gui;
	ChildWindow::Ptr child;
	bool isActive = false;
	ComboBox::Ptr objectSets;
	string lastSelectedObjectSet;
	ComboBox::Ptr objectAssets;
	string lastSelectedObjectAsset;
	ObjectAsset* selectedObjectAsset = nullptr;

	Map* map;

	sf::RectangleShape* selectedObjectAssetRenderer;

};