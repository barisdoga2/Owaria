#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Weapon.h>
#include <WeaponData.h>
#include <Animation.h>
#include <ContactData.h>
#include <ItemAsset.h>

#define COOLDOWN 1000

class Weapon {

public:
	Weapon(ItemAsset* itemAsset, Animation* animation);
	~Weapon();

	void Update(int updateElapsedMs, b2Body* body, int sex, bool yMirror);
	void StartAttack();
	bool IsAttacking();

	Animation* GetAnimation();
	ItemAsset* GetItemAsset();

private:
	bool isAttacking;
	bool isAvailable;
	int countDown;

	ItemAsset* itemAsset;
	ContactData* weaponContact;
	Animation* animation;

	b2Fixture* fixture;

};