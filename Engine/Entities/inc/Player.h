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

#define IDLE "idle"
#define WALK "walk"
#define CLIMB_UP "climbUp"
#define CLIMB_DOWN "climbDown"
#define IDLE "idle"


class Player {
public:
	Player(b2World* world, Map* map, sf::Vector2f worldPosition, int sex);
	~Player();

	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window, Camera camera);
	void HandleInputs(int updateElapsed);
	void HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin);

	// Getters
	sf::Vector2f getPixPosition();
	b2Vec2 getb2Position();
	b2Body* getBody();

	// Static
	static void LoadAssets();
	
	// Goods
	bool WearItem(ItemAsset* itemAsset);
	bool UnWearItem(ItemAsset* itemAsset);
	Item* items[WEARABLE_COUNT] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
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
	sf::Texture* bodySpriteSheet;

	// Animations
	Animation* currentBodyAnimation;
	vector<Animation*> animations;
	void CreateAnimations();
	Animation* GetAnimation(string animationAssetName);

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



};