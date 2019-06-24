#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

class Animation {

public:
	Animation(string name, int xStart, int yStart, int width, int height, int length, int frameDelay, bool isContinuous);
	~Animation();

	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window, sf::Texture* texture, sf::Vector2f coords, int yMirror = false);

	void Hide();
	void Show();
	void Play();
	void Stop();
	bool isFinished();
	string GetName();

private:
	int currentFrame;
	bool isContinuous;
	string name;
	int frameDelay;
	int length;
	int timePassedMs;
	int isPlaying;
	int width;
	int height;
	bool isHidden = false;
	vector<sf::IntRect> frames;
	sf::Sprite renderer;

};