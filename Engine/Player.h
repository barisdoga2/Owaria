#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Player {
public:
	Player();
	~Player();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window);
	void HandleInputs();

	sf::Vector2f position;
	sf::Vector2f velocity;
private:
	sf::Image* spritesheet;
	sf::Texture* texture;
};