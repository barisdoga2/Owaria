#include <ItemAsset.h>


ItemAsset::ItemAsset(XMLElement* itemAssetElement) {
	name = itemAssetElement->Attribute("name");

	eitherSex = itemAssetElement->BoolAttribute("eitherSex");
	cout << eitherSex << endl;
	forMale = itemAssetElement->BoolAttribute("forMale");
	forFemale = itemAssetElement->BoolAttribute("forFemale");

	animatonAsset = AssetStore::GetAnimationSet("player")->getAnimationAsset(itemAssetElement->FirstChildElement("animationAssetRef")->GetText());

	string spriteSheetName = itemAssetElement->FirstChildElement("spriteSheetName")->GetText();
	if (eitherSex) {
		eitherSpriteSheet = new sf::Texture();
		eitherSpriteSheet->loadFromFile(EITHER_SPRITE_PATH"/" + spriteSheetName);

		eitherWeaponData = new WeaponData(name, animatonAsset, *eitherSpriteSheet);
	}
	else {
		if (forMale) {
			maleSpriteSheet = new sf::Texture();
			maleSpriteSheet->loadFromFile(MALE_SPRITE_PATH"/" + spriteSheetName);

			maleWeaponData = new WeaponData(name, animatonAsset, *maleSpriteSheet);
		}

		if(forFemale){
			femaleSpriteSheet = new sf::Texture();
			femaleSpriteSheet->loadFromFile(FEMALE_SPRITE_PATH"/" + spriteSheetName);

			femaleWeaponData = new WeaponData(name, animatonAsset, *femaleSpriteSheet);
		}
	}

}

ItemAsset::~ItemAsset() {
	if (eitherSpriteSheet != nullptr)
		delete eitherSpriteSheet;

	if (maleSpriteSheet != nullptr)
		delete maleSpriteSheet;

	if (femaleSpriteSheet != nullptr)
		delete femaleSpriteSheet;

	if (eitherWeaponData != nullptr)
		delete eitherWeaponData;

	if (maleWeaponData != nullptr)
		delete maleWeaponData;

	if (femaleWeaponData != nullptr)
		delete femaleWeaponData;
}

string ItemAsset::getName() {
	return name;
}

WeaponData* ItemAsset::getWeaponData(int sex) {
	if (eitherSex)
		return eitherWeaponData;

	if (sex == MALE_SEX) {
		if (forMale)
			return maleWeaponData;
		else
			throw "Required item does not support this sex.";
	}
	else if(sex == FEMALE_SEX) {
		if (forFemale) 
			return femaleWeaponData;
		else
			throw "Required item does not support this sex.";
	}
	else {
		throw "Parameter sex doesn't match with any sex.";
	}

	return nullptr;
}

sf::Texture* ItemAsset::getSpriteSheet(int sex) {
	if (eitherSex)
		return eitherSpriteSheet;

	if (sex == MALE_SEX) {
		if (forMale)
			return maleSpriteSheet;
		else
			throw "Required item does not support this sex.";
	}
	else if (sex == FEMALE_SEX) {
		if (forFemale)
			return femaleSpriteSheet;
		else
			throw "Required item does not support this sex.";
	}
	else {
		throw "Parameter sex doesn't match with any sex.";
	}

	return nullptr;
}

AnimationAsset* ItemAsset::getAnimationAsset() {
	return animatonAsset;
}