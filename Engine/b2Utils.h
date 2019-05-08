#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Settings.h>

class b2Utils {
public:
	static b2Fixture* AddRectangleFixture(b2Body* body, int width, int height, int x, int y, float restitution, float density, float friction, bool isSensor = false);
	static b2Fixture* AddCircleFixture(b2Body* body, int radius, int x, int y, float restitution, float density, float friction, bool isSensor = false);
	static b2Fixture* AddChainLoopFixture(b2Body* body, std::vector<sf::Vector2i> t_vertices, float restitution, float density, float friction, bool isSensor = false);
};