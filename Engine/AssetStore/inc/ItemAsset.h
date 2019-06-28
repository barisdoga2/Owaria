#pragma once

#include <iostream>
#include <tinyxml2.h>
class AssetStore;
#include <AssetStore.h>
class WeaponData;
#include <WeaponData.h>

using namespace tinyxml2;
using namespace std;

#define MALE_SEX 1
#define FEMALE_SEX 2

#define BASE_SPRITE_PATH "../../Resources/Items/Sprites"
#define EITHER_SPRITE_PATH BASE_SPRITE_PATH"/either"
#define MALE_SPRITE_PATH BASE_SPRITE_PATH"/male"
#define FEMALE_SPRITE_PATH BASE_SPRITE_PATH"/female"

class ItemAsset {

public:
	ItemAsset(XMLElement* itemAssetElement);
	~ItemAsset();

	int getID();
	string getName();
	AnimationAsset* getAnimationAsset();
	WeaponData* getWeaponData(int sex);
	sf::Texture* getSpriteSheet(int sex);
	sf::Texture* getIcon();

private:
	int id;
	string name;
	bool forMale;
	bool forFemale;
	bool eitherSex;

	AnimationAsset* animatonAsset;

	WeaponData* eitherWeaponData = nullptr;
	WeaponData* maleWeaponData = nullptr;
	WeaponData* femaleWeaponData = nullptr;
	sf::Texture* maleSpriteSheet = nullptr;
	sf::Texture* femaleSpriteSheet = nullptr;
	sf::Texture* eitherSpriteSheet = nullptr;

	sf::Texture* icon = nullptr;

};
