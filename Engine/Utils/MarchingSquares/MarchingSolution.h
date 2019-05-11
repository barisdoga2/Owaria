#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

class MarchingSolution {

public:
	vector<sf::Vector2i> corners;
	vector<sf::Vector2i> edgePoints;
	vector<sf::Vector2i> t_vertices;
	sf::Vector2i lookup;

};