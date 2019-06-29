#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <WeaponData.h>
#include <Animation.h>
#include <ContactData.h>
#include <ItemAsset.h>
#include <Item.h>

#define COOLDOWN 1000

class MeleeWeapon : public Item {

public:
	MeleeWeapon(ItemAsset* itemAsset, Animation* animation);
	~MeleeWeapon();

	void Update(int updateElapsedMs, b2Body* body, int sex, bool yMirror);
	void StartAttack();
	bool IsAttacking();

	Animation* GetAnimation();
	ItemAsset* GetItemAsset();

private:
	bool isAttacking;
	bool isAvailable;
	int countDown;

	ContactData* weaponContact;
	Animation* animation;

	b2Fixture* fixture;

};