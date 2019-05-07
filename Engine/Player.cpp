#include <Player.h>


Player::Player(b2World* world, Map* map, sf::Vector2f worldPosition) {
	this->map = map;

	std::string playerPath = "../../Resources/Player/";

	spritesheet = new sf::Image();
	spritesheet->loadFromFile(playerPath + "Player.png");

	texture = new sf::Texture();
	texture->loadFromImage(*spritesheet, sf::IntRect(0,0,64,64));

	b2BodyDef m_b2BodyDef;
	m_b2BodyDef.type = b2_dynamicBody;
	m_b2BodyDef.position.Set(worldPosition.x / BOX2D_SCALE, worldPosition.y / BOX2D_SCALE);
	body = world->CreateBody(&m_b2BodyDef);
	body->SetFixedRotation(true);

	AddRectangleFixture(10, 10, 19 + 14, 16 + 25, 0, 0, 0)->SetUserData(NULL);

	playerRenderer = new sf::RectangleShape();
	playerRenderer->setSize(sf::Vector2f((float)texture->getSize().x, (float)texture->getSize().y));
	playerRenderer->setPosition(sf::Vector2f(worldPosition.x * BOX2D_SCALE - map->m_offset.x, worldPosition.y * BOX2D_SCALE - map->m_offset.y));
	playerRenderer->setTexture(texture);
}

Player::~Player() {
	delete spritesheet;
	delete texture;
	delete playerRenderer;
}

void Player::Render(sf::RenderWindow* window) {
	b2Vec2 position = body->GetPosition();
	playerRenderer->setPosition(sf::Vector2f(position.x * BOX2D_SCALE - map->m_offset.x, position.y * BOX2D_SCALE - map->m_offset.y));
	window->draw(*playerRenderer);

	
	b2Fixture* fix = body->GetFixtureList();
	sf::Color renderColor = sf::Color(125, 125, 125, 125);
	while (fix != nullptr) {
		if (b2PolygonShape* v = dynamic_cast<b2PolygonShape*>(fix->GetShape())) {

			sf::VertexArray vertices(sf::TrianglesFan, v->GetVertexCount());
			for (int i = 0; i < v->GetVertexCount(); i++)
				vertices[i] = sf::Vertex(sf::Vector2f((v->m_vertices[i].x + body->GetPosition().x) * BOX2D_SCALE - map->m_offset.x, (v->m_vertices[i].y + body->GetPosition().y) * BOX2D_SCALE - map->m_offset.y), renderColor);

			window->draw(vertices);
		}
		fix = fix->GetNext();
	}
}

void Player::Update(int updateElapsed) {
	
}

bool isOnAir = false;
void Player::HandleInputs() {
	int health = 100;
	float PLAYER_SPEED = 1;
	float PLAYER_JUMP_SPEED = 10;
	if (health > 0) {
		b2Vec2 vel = body->GetLinearVelocity();
		b2Vec2 playerMovement = b2Vec2(0, 0);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			playerMovement.x = -PLAYER_SPEED;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			playerMovement.x = PLAYER_SPEED;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !isOnAir) {
			isOnAir = true;
			playerMovement.y = -PLAYER_JUMP_SPEED;
		}
		body->SetLinearVelocity(vel + playerMovement);
	}

	static bool flag = true;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (flag) {


			flag = false;
		}
	}
	else {
		flag = true;
	}
}

b2Fixture* Player::AddRectangleFixture(int width, int height, int x, int y, float restitution, float density, float friction) {
	b2PolygonShape polygonShape;
	b2FixtureDef fixtureDef;
	polygonShape.SetAsBox(width / BOX2D_SCALE, height / BOX2D_SCALE, b2Vec2(x / BOX2D_SCALE, y / BOX2D_SCALE), 0);
	fixtureDef.restitution = restitution;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.shape = &polygonShape;
	return body->CreateFixture(&fixtureDef);
}

b2Fixture* Player::AddCircleFixture(int radius, int x, int y, float restitution, float density, float friction) {
	b2CircleShape circleShape;
	b2FixtureDef fixtureDef;
	circleShape.m_p.Set(x / BOX2D_SCALE, y / BOX2D_SCALE);
	circleShape.m_radius = radius / BOX2D_SCALE;
	fixtureDef.restitution = restitution;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.shape = &circleShape;
	return body->CreateFixture(&fixtureDef);
}