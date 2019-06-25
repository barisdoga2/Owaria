#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

class Animation {

public:
	Animation(string name, sf::Vector2i startPosition, sf::Vector2i size, int length, int frameDelay, bool isContinuous);
	~Animation();

	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window, sf::Texture* texture, sf::Vector2f coords, int yMirror = false);

	void Hide();
	void Show();
	void Play();
	void Stop();
	bool isFinished();

	int GetCurrentFrame();

	sf::Vector2i getStartPosition();
	sf::Vector2i getSize();
	vector<sf::Vector2i> GetFrameCoords();
	string GetName();

private:
	int currentFrame;
	bool isContinuous;
	string name;
	sf::Vector2i startPosition;
	sf::Vector2i size;
	int frameDelay;
	int length;
	int timePassedMs;
	int isPlaying;
	bool isHidden = false;
	vector<sf::Vector2i> frameCoords;
	sf::Sprite renderer;

};