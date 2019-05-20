#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <Animation.h>
#include <ioUtils.h>

using namespace std;

class Effect {

public:
	Effect(string effectName);
	~Effect();

	void Start();
	void Render(sf::RenderWindow* window, sf::Vector2f offset);
	void Update(int updateElapsed);

	Effect* getClone();

private:
	string effectName;
	Animation* effectAnim;
	sf::Image* img;

};