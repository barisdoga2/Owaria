#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <AnimationAsset.h>
#include <b2Utils.h>


class WeaponData {

public:
	WeaponData(std::string name, AnimationAsset* animationAsset, sf::Texture spriteSheet);
	~WeaponData();

	vector<sf::Vector2i> GetFramePoints(int frameNum, bool yMirror = false);

private:
	std::string name;
	vector<vector<sf::Vector2i>> frame_points;
	vector<vector<sf::Vector2i>> frame_points_yMirror;

};