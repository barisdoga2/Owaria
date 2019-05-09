#include <Player.h>


Player::Player(b2World* world, Map* map, sf::Vector2f worldPosition) {
	this->map = map;

	std::string playerPath = "../../Resources/Player/";

	// Create Animations
	spritesheet = new sf::Image();
	spritesheet->loadFromFile(playerPath + "Player.png");

	walkAnimation = new Animation(spritesheet, "walk", 9, 64, 64, 9, 300, true);
	walkAnimation->Play();

	jumpAnimation = new Animation(spritesheet, "jump", 5, 64, 64, 3, 400, false);
	jumpAnimation->Play();

	idleAnimation = new Animation(spritesheet, "idle", 1, 64, 64, 2, 680, true);
	idleAnimation->Play();
	
	currentAnimation = idleAnimation;


	// Create Physics
	b2BodyDef m_b2BodyDef;
	m_b2BodyDef.type = b2_dynamicBody;
	m_b2BodyDef.position.Set(worldPosition.x / BOX2D_SCALE, worldPosition.y / BOX2D_SCALE);
	body = world->CreateBody(&m_b2BodyDef);
	body->SetFixedRotation(true);

	// Create ContactData
	ContactData* bodyContact = new ContactData(CONTACT_TYPE_PLAYER_INSTANCE, this);
	ContactData* footContact = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)FOOT_SENSOR);
	body->SetUserData((void*)bodyContact);

	b2Utils::AddRectangleFixture(body, 2, 15, 25, 35, 0, 0, 0)->SetUserData((void*)bodyContact); // Left Cover for not stuck on the walls, friction = 0
	b2Utils::AddRectangleFixture(body, 10, 15, 27, 37, 0, PLAYER_DENSITY, PLAYER_FRICTION)->SetUserData((void*)bodyContact); // Middle
	b2Utils::AddRectangleFixture(body, 2, 15, 37, 35, 0, 0, 0)->SetUserData((void*)bodyContact); // Right Cover for not stuck on the walls, friction = 0

	b2Utils::AddRectangleFixture(body, 4, 4, 26, 52, 0, 0, 0, true)->SetUserData((void*)footContact); // left feet
	b2Utils::AddRectangleFixture(body, 4, 2, 30, 55, 0, 0, 0, true)->SetUserData((void*)footContact); // mid feet
	b2Utils::AddRectangleFixture(body, 4, 4, 34, 52, 0, 0, 0, true)->SetUserData((void*)footContact); // right feet

	b2Utils::AddCircleFixture(body, 6, 26, 45, 0, PLAYER_DENSITY / 2, PLAYER_FRICTION)->SetUserData((void*)bodyContact); // legs
	b2Utils::AddCircleFixture(body, 6, 26, 13, 0, PLAYER_DENSITY / 2, 0)->SetUserData((void*)bodyContact); // head
}

Player::~Player() {
	delete spritesheet;
	delete walkAnimation;
	delete idleAnimation;
	delete jumpAnimation;
}

void Player::Render(sf::RenderWindow* window) {
	b2Vec2 position = body->GetPosition();

	currentAnimation->Render(window, sf::Vector2f(position.x * BOX2D_SCALE - map->m_offset.x, position.y * BOX2D_SCALE - map->m_offset.y), moveDirection == 1);

	b2Utils::RenderFixtures(window, body, map->m_offset);
	
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
	
}

void Player::HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin) {
	
	if (((ContactData*)self->GetUserData())->getDataType() == CONTACT_TYPE_SENSOR_INT) {
		if ((int)((ContactData*)self->GetUserData())->getData() == FOOT_SENSOR) {
			if (isBegin)
				numFootContacts++;
			else
				numFootContacts--;

			if (numFootContacts > 0) {
				isOnAir = false;
			}
			else {
				isOnAir = true;
			}
		}
	}

}