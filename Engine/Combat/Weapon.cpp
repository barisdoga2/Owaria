#include <Weapon.h>

Weapon::Weapon(std::string name, b2Body* body, std::string spriteSheetPath, Animation* animationToExtractWeaponData) {
	this->body = body;
	this->animation = animationToExtractWeaponData;

	spriteSheet = new sf::Texture();
	spriteSheet->loadFromFile(spriteSheetPath);
	
	weaponData = new WeaponData("dagger", animation->getAnimationAsset(), *spriteSheet); // Extract Weapon Data from Animation Frames and Sprite Sheet

	weaponContact = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)WEAPON_SENSOR);

	fixture = nullptr;

	isAttacking = false;
	isAvailable = true;
}

Weapon::~Weapon() {
	delete spriteSheet;
	delete weaponData;
	delete weaponContact;
}

void Weapon::Update(int updateElapsedMs, bool yMirror) {
	countDown -= updateElapsedMs;
	isAvailable = countDown <= 0;

	if (isAttacking) {
		if (fixture != nullptr)
			body->DestroyFixture(fixture);

		fixture = b2Utils::AddChainLoopFixture(body, weaponData->GetFramePoints(animation->GetCurrentFrame(), yMirror), 0, 0, 0, true);
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

sf::Texture* Weapon::GetSpriteSheet() {
	return this->spriteSheet;
}