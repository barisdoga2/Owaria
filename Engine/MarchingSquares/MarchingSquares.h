#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Map.h>
#include <MarchingSolution.h>

using namespace std;


class MarchingSquares {

public:
	MarchingSquares(Map* map);

	vector<MarchingSolution> solutions;
	

private:
	Map* map;
	int* bitmap;

	vector<sf::Vector2i> lookups;
	vector<sf::Vector2i> lookupDirs;

	void generateBitmapFromMap();
	void findLookups();
	void solveAll();

	MarchingSolution solve(sf::Vector2i lookup, sf::Vector2i lookupDir);

};