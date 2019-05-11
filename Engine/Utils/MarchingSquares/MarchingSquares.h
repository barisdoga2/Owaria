#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
class Map;
#include <Map.h>
#include <MarchingSolution.h>

using namespace std;


class MarchingSquares {

public:
	MarchingSquares(Map* map);

	vector<MarchingSolution> solutions;
	

private:
	Map* map;

	vector<sf::Vector2i> lookups;
	vector<sf::Vector2i> lookupDirs;

	void findLookups();
	void solveAll();
	void pushSpecialTile(MarchingSolution* solution, TileData* tD, int x, int y, int xCheckDir, int yCheckDir, int axisSwap);
	void pushSpecialTileCorner(MarchingSolution* solution, TileData* tD, int x, int y, int xCheckDir, int yCheckDir, int axisSwap);

	MarchingSolution solve(sf::Vector2i lookup, sf::Vector2i lookupDir);

};