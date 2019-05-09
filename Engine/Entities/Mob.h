#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Map.h>
#include <Settings.h>
#include <Animation.h>
#include <b2Utils.h>
#include <ContactData.h>

class Mob {
public:
	Mob(b2World* world, Map* map, sf::Vector2f worldPosition);
	~Mob();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window);

	sf::Vector2f position;
	sf::Vector2f velocity;

	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape s;
	b2FixtureDef fixtureDef;

	int moveDirection = 1;
	bool isOnAir = false;
	int numFootContacts = 0;

	void HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin);

private:
	Map * map;
	Animation* currentAnimation;
	Animation* walkAnimation;
	Animation* idleAnimation;
	sf::Image* spritesheet;
};