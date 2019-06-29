#include <MeleeWeapon.h>

MeleeWeapon::MeleeWeapon(ItemAsset* itemAsset, Animation* animation) : Item(itemAsset) {
	this->animation = animation;
	
	weaponContact = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)WEAPON_SENSOR);

	fixture = nullptr;

	isAttacking = false;
	isAvailable = true;
}

MeleeWeapon::~MeleeWeapon() {
	delete weaponContact;
}

void MeleeWeapon::Update(int updateElapsedMs, b2Body* body, int sex, bool yMirror) {
	if (animation == nullptr)
		return;

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

void MeleeWeapon::StartAttack() {
	if (isAvailable) {
		isAttacking = true;
		isAvailable = false;
		countDown = COOLDOWN;
	}
}

bool MeleeWeapon::IsAttacking() {
	return isAttacking;
}

ItemAsset* MeleeWeapon::GetItemAsset() {
	return itemAsset;
}

Animation* MeleeWeapon::GetAnimation() {
	return animation;
}