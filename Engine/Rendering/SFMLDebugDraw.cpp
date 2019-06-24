#include "SFMLDebugDraw.h"

SFMLDebugDraw::SFMLDebugDraw(sf::RenderWindow* window, b2World* world, Camera* camera) {
	this->m_window = window;
	this->world = world;
	this->camera = camera;

	cout << "Press F1 to enable or disable Box2D Debug Draw!" << endl;

	this->SetFlags(b2Draw::e_shapeBit);
	world->SetDebugDraw(this);
}

void SFMLDebugDraw::Render() {
	if (isDebugDrawEnabled) 
		world->DrawDebugData();

	// Check Debug Draw is Enable or Disable Event
	static bool releaseFlag = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
		if (releaseFlag)
			releaseFlag = false;
	}
	else {
		if (!releaseFlag) {
			isDebugDrawEnabled = !isDebugDrawEnabled;
			releaseFlag = true;
		}
	}
}

void SFMLDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	sf::ConvexShape polygon(vertexCount);
	sf::Vector2f center;
	for(int i = 0; i < vertexCount; i++)
	{
		sf::Vector2f transformedVec = B2VecToSFVec(vertices[i]);
		polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y)));
	}
	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(sf::Color::Transparent);
	polygon.setOutlineColor(sf::Color::Red);

	m_window->draw(polygon);
}

void SFMLDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	sf::ConvexShape polygon(vertexCount);
	for(int i = 0; i < vertexCount; i++)
	{
		sf::Vector2f transformedVec = B2VecToSFVec(vertices[i]);
		polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y)));
	}
	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(SFMLDebugDraw::GLColorToSFML(color, 60));
	polygon.setOutlineColor(sf::Color::Red);

	m_window->draw(polygon);
}

void SFMLDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
	sf::CircleShape circle(radius * BOX2D_SCALE);
	circle.setOrigin(radius * BOX2D_SCALE, radius * BOX2D_SCALE);
	circle.setPosition(B2VecToSFVec(center));
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(-1.f);
	circle.setOutlineColor(sf::Color::Red);
	
	// This is disabled because it is rendering loop chain fixture's loop circle!
	//m_window->draw(circle);
}

void SFMLDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
	sf::CircleShape circle(radius * BOX2D_SCALE);
	circle.setOrigin(radius * BOX2D_SCALE, radius * BOX2D_SCALE);
	circle.setPosition(B2VecToSFVec(center));
	circle.setFillColor(SFMLDebugDraw::GLColorToSFML(color, 60));
	circle.setOutlineThickness(1.f);
	circle.setOutlineColor(sf::Color::Red);

	b2Vec2 endPoint = center + radius * axis;
	sf::Vertex line[2] = 
	{
		sf::Vertex(B2VecToSFVec(center), SFMLDebugDraw::GLColorToSFML(color)),
		sf::Vertex(B2VecToSFVec(endPoint), SFMLDebugDraw::GLColorToSFML(color)),
	};

	m_window->draw(circle);
	m_window->draw(line, 2, sf::Lines);
}

void SFMLDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	sf::Vertex line[] =
	{
		sf::Vertex(B2VecToSFVec(p1), sf::Color::Red),
		sf::Vertex(B2VecToSFVec(p2), sf::Color::Red)
	};

	m_window->draw(line, 2, sf::Lines);
}

void SFMLDebugDraw::DrawTransform(const b2Transform& xf) {
	float lineLength = 0.4f;

	b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
	sf::Vertex redLine[] = 
	{
		sf::Vertex(B2VecToSFVec(xf.p), sf::Color::Red),
		sf::Vertex(B2VecToSFVec(xAxis), sf::Color::Red)
	};

	b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
	sf::Vertex greenLine[] = 
	{
		sf::Vertex(B2VecToSFVec(xf.p), sf::Color::Green),
		sf::Vertex(B2VecToSFVec(yAxis), sf::Color::Green)
	};

	m_window->draw(redLine, 2, sf::Lines);
	m_window->draw(greenLine, 2, sf::Lines);
}

// TODO : Implement this
void SFMLDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {

}