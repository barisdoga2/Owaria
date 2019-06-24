#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Map.h>
#include <Settings.h>
#include <b2Utils.h>
#include <ContactData.h>
#include <Animation.h>
class WeaponData;
#include <WeaponData.h>

class Player {
public:
	Player(b2World* world, Map* map, sf::Vector2f worldPosition);
	~Player();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window, Camera camera);

	void HandleInputs(int updateElapsed);
	void HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin);

	b2Body* body_foot;
	b2Body* body;

	sf::Vector2f moveDirection = sf::Vector2f(1, 0);
	int numFootContacts = 0;
	int numLadderContacts = 0;

	bool isOnLadder = false;
	bool isOnAir = false;
	bool isWalking = false;
	bool isAttacking = false;
	bool isClimbing = false;

	sf::Vector2f getPixPosition();
	b2Vec2 getb2Position();

private:
	Map * map;
	Animation* currentAnimation;
	Animation* walkAnimation;
	Animation* idleAnimation;
	Animation* slashAnimation;
	Animation* climbUpAnimation;
	Animation* climbDownAnimation;
	sf::Texture* spritesheet;
	sf::Texture* spritesheet2;

	b2RevoluteJoint* foot_joint;

	b2Fixture* dagger;
	WeaponData* weaponData;
};