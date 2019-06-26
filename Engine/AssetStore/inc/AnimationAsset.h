#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace std;

class AnimationAsset {

public:
	AnimationAsset(string name, sf::Vector2i startPosition, sf::Vector2i size, int frameCount, int frameDelay);
	~AnimationAsset();

	string getName();
	vector<sf::Vector2i> getFrameCoords();
	sf::Vector2i getSize();
	int getFrameCount();
	int getFrameDelay();

private:
	string name;
	sf::Vector2i startPosition;
	sf::Vector2i size;
	int frameCount;
	int frameDelay;
	vector<sf::Vector2i> frameCoords;

};