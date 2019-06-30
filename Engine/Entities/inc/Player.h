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
#include <MeleeWeapon.h>
#include <Armor.h>
#include <Item.h>
class Inventory;
#include <Inventory.h>

#define IDLE "idle"
#define WALK "walk"
#define CLIMB_UP "climbUp"
#define CLIMB_DOWN "climbDown"
#define IDLE "idle"


class Player {
public:
	Player(sf::RenderWindow* window, b2World* world, Map* map, sf::Vector2f worldPosition, int sex);
	~Player();

	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window, Camera camera);
	void HandleInputs(int updateElapsed);
	void HandleWindowEvent(sf::Event event);
	void HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin);

	// Getters
	sf::Vector2f getPixPosition();
	b2Vec2 getb2Position();
	b2Body* getBody();
	Animation* GetAnimation(string animationAssetName);

	// Static
	static void LoadAssets();

private:
	int sex;
	Map * map;
	sf::Texture* bodySpriteSheet;

	// Animations
	Animation* currentBodyAnimation;
	vector<Animation*> animations;
	void CreateAnimations();

	// Physics
	b2Body* body_foot;
	b2Body* body;
	b2RevoluteJoint* foot_joint;
	sf::Vector2i moveDirection = sf::Vector2i(1, 0);
	int numFootContacts = 0;
	int numLadderContacts = 0;
	bool isOnLadder = false;
	bool isOnAir = false;
	bool isWalking = false;
	bool isClimbing = false;
	void CreatePhysics(b2World* world, sf::Vector2f worldPosition);

	// Inventory
	Inventory* inventory;

};