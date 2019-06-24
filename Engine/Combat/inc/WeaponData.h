#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
class Animation;
#include <Animation.h>


class WeaponData {

public:
	WeaponData(std::string name, Animation animation, sf::Texture spriteSheet);
	~WeaponData();

	std::string name;
	vector<vector<sf::Vector3i>> frame_points;
private:

};