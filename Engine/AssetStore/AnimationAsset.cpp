#include <AnimationAsset.h>


AnimationAsset::AnimationAsset(string name, sf::Vector2i startPosition, sf::Vector2i size, int frameCount, int frameDelay, bool continious) {
	this->name = name;
	this->startPosition = startPosition;
	this->size = size;
	this->frameCount = frameCount;
	this->frameDelay = frameDelay;
	this->continious = continious;

	for (int i = 0; i < frameCount; i++)
		frameCoords.push_back(sf::Vector2i(startPosition.x + i * size.x, startPosition.y));
}

AnimationAsset::~AnimationAsset() {

}

string AnimationAsset::getName() {
	return name;
}

vector<sf::Vector2i> AnimationAsset::getFrameCoords() {
	return frameCoords;
}

int AnimationAsset::getFrameCount() {
	return frameCount;
}

sf::Vector2i AnimationAsset::getSize() {
	return size;
}

int AnimationAsset::getFrameDelay() {
	return frameDelay;
}

bool AnimationAsset::isContinious(){
	return continious;
}