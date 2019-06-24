#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Settings.h>


class b2Utils
{

public:
	static b2Fixture * b2Utils::AddRectangleFixture(b2Body* body, int width, int height, int x, int y, float restitution, float density, float friction, bool isSensor = 0) {
		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;
		polygonShape.SetAsBox(width / BOX2D_SCALE / 2, height / BOX2D_SCALE / 2, b2Vec2((x + width / 2) / BOX2D_SCALE, (y + height / 2) / BOX2D_SCALE), 0);
		fixtureDef.restitution = restitution;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.shape = &polygonShape;
		fixtureDef.isSensor = isSensor;
		return body->CreateFixture(&fixtureDef);
	}

	static b2Fixture* b2Utils::AddCircleFixture(b2Body* body, int radius, int x, int y, float restitution, float density, float friction, bool isSensor = 0) {
		b2CircleShape circleShape;
		b2FixtureDef fixtureDef;
		circleShape.m_p.Set((x + radius) / BOX2D_SCALE, (y + radius) / BOX2D_SCALE);
		circleShape.m_radius = radius / BOX2D_SCALE;
		fixtureDef.restitution = restitution;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.shape = &circleShape;
		fixtureDef.isSensor = isSensor;
		return body->CreateFixture(&fixtureDef);
	}

	static b2Fixture* b2Utils::AddChainLoopFixture(b2Body* body, std::vector<sf::Vector2i> t_vertices, float restitution, float density, float friction, bool isSensor = 0) {
		b2ChainShape chainShape;

		b2Vec2* v = (b2Vec2*)malloc(t_vertices.size() * sizeof(b2Vec2));
		for (unsigned int i = 0; i < t_vertices.size(); i++)
			*(v + i) = b2Vec2(t_vertices.at(i).x / BOX2D_SCALE, t_vertices.at(i).y / BOX2D_SCALE);
		chainShape.CreateLoop(v, t_vertices.size());
		free(v);

		b2FixtureDef fixtureDef;
		fixtureDef.restitution = restitution;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.shape = &chainShape;
		fixtureDef.isSensor = isSensor;

		return body->CreateFixture(&fixtureDef);
	}

};