#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Weapon.h>
#include <WeaponData.h>
#include <Animation.h>
#include <ContactData.h>

#define COOLDOWN 1000

class Weapon {

public:
	Weapon(std::string name, b2Body* body, std::string spriteSheetPath, Animation* animationToExtractWeaponData);
	~Weapon();

	void Update(int updateElapsedMs, bool yMirror);
	void StartAttack();
	bool IsAttacking();

	sf::Texture* GetSpriteSheet();

private:
	bool isAttacking;
	bool isAvailable;
	int countDown;

	sf::Texture* spriteSheet;
	WeaponData* weaponData;
	ContactData* weaponContact;
	Animation* animation;

	b2Fixture* fixture;
	b2Body* body;
};