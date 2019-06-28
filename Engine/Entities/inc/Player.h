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
#include <Weapon.h>



class Player {
public:
	Player(b2World* world, Map* map, sf::Vector2f worldPosition, int sex);
	~Player();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window, Camera camera);

	void HandleInputs(int updateElapsed);
	void HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin);

	b2Body* body_foot;
	b2Body* body;

	sf::Vector2i moveDirection = sf::Vector2i(1, 0);
	int numFootContacts = 0;
	int numLadderContacts = 0;

	bool isOnLadder = false;
	bool isOnAir = false;
	bool isWalking = false;
	bool isClimbing = false;

	sf::Vector2f getPixPosition();
	b2Vec2 getb2Position();

	static void LoadAssets();

	int inventory[5 * 10] = {
		1,	2,	0,	0,	0,
		11,	12,	13,	14,	0,
		20,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,
		0,	0,	0,	0,	0
	};

private:
	int sex;

	Map * map;
	Animation* currentBodyAnimation;
	Animation* walkAnimation;
	Animation* idleAnimation;
	Animation* slashAnimation;
	Animation* thrustAnimation;
	Animation* climbUpAnimation;
	Animation* climbDownAnimation;
	sf::Texture* bodySpriteSheet;

	b2RevoluteJoint* foot_joint;

	Weapon* daggerWeapon = nullptr;

};