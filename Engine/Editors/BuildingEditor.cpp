#include <BuildingEditor.h>



BuildingEditor::BuildingEditor(sf::RenderWindow* window, Map* map) {
	cout << "Press 'CTRL+B' or '3' to open or close Building Editor! While it is opened you can press 'H' to hide or show building selector!" << endl;
	this->window = window;
	this->map = map;

	gui = new Gui(*window);

}

BuildingEditor::~BuildingEditor() {
	delete gui;
}

void BuildingEditor::Update(int updateElapsed, Camera* camera) {
	static bool keyRelaseFlag = false;
	if (((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) && sf::Keyboard::isKeyPressed(sf::Keyboard::B)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))) {
		if (keyRelaseFlag) {
			isActive = !isActive;
			camera->SetFreeRoam(isActive);
			keyRelaseFlag = false;
		}
	}
	else {
		keyRelaseFlag = true;
	}

	if (!isActive)
		return;

	static bool keyRelaseFlag2 = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
		if (keyRelaseFlag2) {
			// Hide or show
			keyRelaseFlag2 = false;
		}
	}
	else {
		keyRelaseFlag2 = true;
	}

}

void BuildingEditor::Render(Camera* camera) {
	if (!isActive)
		return;
	gui->draw();
	
}

void BuildingEditor::HandleWindowEvent(sf::Event event, Camera* camera) {
	if (!isActive)
		return;

	gui->handleEvent(event);

	sf::Vector2i mPos = sf::Mouse::getPosition(*window);
	
}

void BuildingEditor::itemSelectCallback() {
	
}