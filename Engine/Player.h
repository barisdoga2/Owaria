#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Map.h>
#include <Settings.h>

class Player {
public:
	Player(b2World* world, Map* map, sf::Vector2f worldPosition);
	~Player();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window);
	void HandleInputs();

	sf::Vector2f position;
	sf::Vector2f velocity;

	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape s;
	b2FixtureDef fixtureDef;

private:
	Map * map;
	sf::Image* spritesheet;
	sf::Texture* texture;
	sf::RectangleShape* playerRenderer;
	b2Fixture* AddRectangleFixture(int width, int height, int x, int y, float restitution, float density, float friction);
	b2Fixture* AddCircleFixture(int radius, int x, int y, float restitution, float density, float friction);
};