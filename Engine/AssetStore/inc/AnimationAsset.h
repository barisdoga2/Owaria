#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace std;

class AnimationAsset {

public:
	AnimationAsset(string name, sf::Vector2i startPosition, sf::Vector2i size, int frameCount, int frameDelay, bool continious);
	~AnimationAsset();

	string getName();
	vector<sf::Vector2i> getFrameCoords();
	sf::Vector2i getSize();
	int getFrameCount();
	int getFrameDelay();
	bool isContinious();

private:
	string name;
	sf::Vector2i startPosition;
	sf::Vector2i size;
	bool continious;
	int frameCount;
	int frameDelay;
	vector<sf::Vector2i> frameCoords;

};