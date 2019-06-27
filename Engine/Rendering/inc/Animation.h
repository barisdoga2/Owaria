#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <AnimationAsset.h>

using namespace std;

class Animation {

public:
	Animation(AnimationAsset* animationAsset, bool isContinuous);
	~Animation();

	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window, sf::Texture* texture, sf::Vector2f coords, int yMirror = false);

	void Hide();
	void Show();
	void Play();
	void Stop();
	bool isFinished();

	int GetCurrentFrame();

	AnimationAsset* getAnimationAsset();
	string getAnimationAssetName();

private:
	AnimationAsset* animationAsset;

	int currentFrame;
	bool isContinuous;
	int timePassedMs;
	int isPlaying;
	bool isHidden = false;
	sf::Sprite renderer;

};