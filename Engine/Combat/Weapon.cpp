#include <Weapon.h>

Weapon::Weapon(ItemAsset* itemAsset, Animation* animation) {
	this->animation = animation;
	this->itemAsset = itemAsset;
	
	weaponContact = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)WEAPON_SENSOR);

	fixture = nullptr;

	isAttacking = false;
	isAvailable = true;
}

Weapon::~Weapon() {
	delete weaponContact;
}

void Weapon::Update(int updateElapsedMs, b2Body* body, int sex, bool yMirror) {
	countDown -= updateElapsedMs;
	isAvailable = countDown <= 0;

	if (isAttacking) {
		if (fixture != nullptr)
			body->DestroyFixture(fixture);
		
		fixture = b2Utils::AddChainLoopFixture(body, itemAsset->getWeaponData(sex)->GetFramePoints(animation->GetCurrentFrame(), yMirror), 0, 0, 0, true);
		fixture->SetUserData((void*)weaponContact);
	}
	else {
		if (fixture != nullptr) {
			body->DestroyFixture(fixture);
			fixture = nullptr;
		}
	}
	
	isAttacking = !animation->isFinished();
}

void Weapon::StartAttack() {
	if (isAvailable) {
		isAttacking = true;
		isAvailable = false;
		countDown = COOLDOWN;
	}
}

bool Weapon::IsAttacking() {
	return isAttacking;
}

ItemAsset* Weapon::GetItemAsset() {
	return itemAsset;
}

Animation* Weapon::GetAnimation() {
	return animation;
}