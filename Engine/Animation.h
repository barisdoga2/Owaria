#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class Animation {

public:
	Animation(sf::Image* image, string name, int yStart, int width, int height, int length, int frameDelay, bool isContinuous);
	~Animation();
	
	void Play();
	void Stop();

	// hide and show is for effects
	void Hide();
	void Show();
	bool isFinished();

	string GetName();
	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window, sf::Vector2f coords, int yMirror = false);

private:
	bool isContinuous;
	string name;
	int frameDelay;
	int length;
	int currentFrame;
	int timePassedMs;
	int isPlaying;
	int width;
	int height;
	bool isHidden = false;
	vector<sf::Texture*> frames;
	sf::Sprite renderer;
};