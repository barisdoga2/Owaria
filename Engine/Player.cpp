#include <Player.h>


Player::Player(b2World* world, Map* map, sf::Vector2f worldPosition) {
	this->map = map;

	std::string playerPath = "../../Resources/Player/";

	spritesheet = new sf::Image();
	spritesheet->loadFromFile(playerPath + "Player.png");


	walkAnimation = new Animation(spritesheet, "walk", 9, 64, 64, 9, 300, true);
	walkAnimation->Play();

	jumpAnimation = new Animation(spritesheet, "jump", 5, 64, 64, 3, 400, false);
	jumpAnimation->Play();

	idleAnimation = new Animation(spritesheet, "idle", 1, 64, 64, 2, 680, true);
	idleAnimation->Play();
	
	currentAnimation = idleAnimation;


	b2BodyDef m_b2BodyDef;
	m_b2BodyDef.type = b2_dynamicBody;
	m_b2BodyDef.position.Set(worldPosition.x / BOX2D_SCALE, worldPosition.y / BOX2D_SCALE);
	body = world->CreateBody(&m_b2BodyDef);
	body->SetFixedRotation(true);

	b2Utils::AddRectangleFixture(body, 2, 15, 25, 35, 0, 0, 0)->SetUserData(NULL); // Left Cover for not stuck on the walls, friction = 0
	b2Utils::AddRectangleFixture(body, 10, 15, 27, 37, 0, PLAYER_DENSITY, PLAYER_FRICTION)->SetUserData(NULL); // Middle
	b2Utils::AddRectangleFixture(body, 2, 15, 37, 35, 0, 0, 0)->SetUserData(NULL); // Right Cover for not stuck on the walls, friction = 0

	contactListener = new MyContactListener(this);
	world->SetContactListener(contactListener);
	b2Utils::AddRectangleFixture(body, 4, 4, 26, 52, 0, 0, 0, true)->SetUserData((void*)PLAYER_FOOT_B2_USER_DATA); // left feet
	b2Utils::AddRectangleFixture(body, 4, 2, 30, 55, 0, 0, 0, true)->SetUserData((void*)PLAYER_FOOT_B2_USER_DATA); // mid feet
	b2Utils::AddRectangleFixture(body, 4, 4, 34, 52, 0, 0, 0, true)->SetUserData((void*)PLAYER_FOOT_B2_USER_DATA); // right feet

	b2Utils::AddCircleFixture(body, 6, 26, 45, 0, PLAYER_DENSITY / 2, PLAYER_FRICTION)->SetUserData(NULL); // legs
	b2Utils::AddCircleFixture(body, 6, 26, 13, 0, PLAYER_DENSITY / 2, 0)->SetUserData(NULL); // head
}

Player::~Player() {
	delete spritesheet;
	delete contactListener;
	delete walkAnimation;
	delete idleAnimation;
	delete jumpAnimation;
}

void Player::Render(sf::RenderWindow* window) {
	b2Vec2 position = body->GetPosition();

	currentAnimation->Render(window, sf::Vector2f(position.x * BOX2D_SCALE - map->m_offset.x, position.y * BOX2D_SCALE - map->m_offset.y), moveDirection == 1);

	b2Fixture* fix = body->GetFixtureList();
	sf::Color renderColor = sf::Color(255, 0, 0, 50);
	while (fix != nullptr) {
		if (b2PolygonShape* v = dynamic_cast<b2PolygonShape*>(fix->GetShape())) {

			sf::VertexArray vertices(sf::TrianglesFan, v->GetVertexCount());
			for (int i = 0; i < v->GetVertexCount(); i++)
				vertices[i] = sf::Vertex(sf::Vector2f((v->m_vertices[i].x + body->GetPosition().x) * BOX2D_SCALE - map->m_offset.x, (v->m_vertices[i].y + body->GetPosition().y) * BOX2D_SCALE - map->m_offset.y), renderColor);

			window->draw(vertices);
		}
		else if (b2CircleShape* v = dynamic_cast<b2CircleShape*>(fix->GetShape())) {
			sf::CircleShape c;
			c.setRadius(v->m_radius * BOX2D_SCALE);
			c.setFillColor(renderColor);
			c.setPosition((v->m_p.x + body->GetPosition().x) * BOX2D_SCALE - c.getRadius() - map->m_offset.x, (v->m_p.y + body->GetPosition().y) * BOX2D_SCALE - c.getRadius() - map->m_offset.y);

			window->draw(c);
		}
		fix = fix->GetNext();
	}
	
}

void Player::Update(int updateElapsed) {
	currentAnimation->Update(updateElapsed);
}

void Player::HandleInputs(int updateElapsed) {
	b2Vec2 vel = body->GetLinearVelocity();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		currentAnimation = walkAnimation;
		moveDirection = -1;
		vel.x = -PLAYER_SPEED;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		currentAnimation = walkAnimation;
		moveDirection = 1;
		vel.x = PLAYER_SPEED;
	}
	else {
		currentAnimation = idleAnimation;
		vel.x = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !isOnAir) {
		vel.y = -PLAYER_JUMP_SPEED;
		isOnAir = true;
	}
	body->SetLinearVelocity(vel);
	
	if (numFootContacts > 0) {
		isOnAir = false;
	}
	else {
		isOnAir = true;
		currentAnimation = jumpAnimation;
	}
}