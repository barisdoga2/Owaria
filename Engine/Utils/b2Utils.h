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

	static void b2Utils::RenderFixtures(sf::RenderWindow* window, b2Body* body, sf::Vector2f map_offset) {
		b2Fixture* fix = body->GetFixtureList();
		b2Vec2 position = body->GetPosition();
		sf::Color renderColor;

		while (fix != nullptr) {
			if (b2PolygonShape* v = dynamic_cast<b2PolygonShape*>(fix->GetShape())) {
				renderColor = sf::Color(0, 0, 255, 70);

				sf::VertexArray vertices(sf::TrianglesFan, v->GetVertexCount());
				for (int i = 0; i < v->GetVertexCount(); i++)
					vertices[i] = sf::Vertex(sf::Vector2f((v->m_vertices[i].x + body->GetPosition().x) * BOX2D_SCALE - map_offset.x, (v->m_vertices[i].y + body->GetPosition().y) * BOX2D_SCALE - map_offset.y), renderColor);

				window->draw(vertices);
			}
			else if (b2CircleShape* v = dynamic_cast<b2CircleShape*>(fix->GetShape())) {
				renderColor = sf::Color(0, 255, 0, 70);

				sf::CircleShape c;
				c.setRadius(v->m_radius * BOX2D_SCALE);
				c.setFillColor(renderColor);
				c.setPosition((v->m_p.x + body->GetPosition().x) * BOX2D_SCALE - c.getRadius() - map_offset.x, (v->m_p.y + body->GetPosition().y) * BOX2D_SCALE - c.getRadius() - map_offset.y);

				window->draw(c);
			}
			else if (b2ChainShape* v = dynamic_cast<b2ChainShape*>(fix->GetShape())) {
				renderColor = sf::Color(255, 0, 0, 255);
				sf::VertexArray vertices(sf::LinesStrip, v->m_count);
				for (int i = 0; i < v->m_count; i++)
					vertices[i] = sf::Vertex(sf::Vector2f((v->m_vertices[i].x + body->GetPosition().x) * BOX2D_SCALE - map_offset.x, (v->m_vertices[i].y + body->GetPosition().y) * BOX2D_SCALE - map_offset.y), renderColor);

				window->draw(vertices);

				
			}
			fix = fix->GetNext();
		}
	}
};