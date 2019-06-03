#include <Player.h>


Player::Player(b2World* world, Map* map, sf::Vector2f worldPosition) {
	this->map = map;

	std::string playerPath = "../../Resources/Player/";

	// Create Animations
	spritesheet = new sf::Image();
	spritesheet->loadFromFile(playerPath + "Player2.png");

	walkAnimation = new Animation(spritesheet, "walk", 9, 64, 64, 9, 300, true);
	walkAnimation->Play();

	idleAnimation = new Animation(spritesheet, "idle", 1, 64, 64, 2, 680, true);
	idleAnimation->Play();
	
	currentAnimation = idleAnimation;


	// Create Physics
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(worldPosition.x / BOX2D_SCALE, worldPosition.y / BOX2D_SCALE);
	body = world->CreateBody(&bodyDef);
	body->SetFixedRotation(true);

	// Create ContactData
	ContactData* bodyContact = new ContactData(CONTACT_TYPE_PLAYER_INSTANCE, this);
	ContactData* footContact = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)FOOT_SENSOR);
	body->SetUserData((void*)bodyContact);

	// Body
	b2Utils::AddRectangleFixture(body, 2, 21, 24, 32, 0, 0, 0)->SetUserData((void*)bodyContact); // Left Cover for not stuck on the walls, friction = 0
	b2Utils::AddRectangleFixture(body, 12, 21, 26, 32, 0, PLAYER_DENSITY, PLAYER_FRICTION)->SetUserData((void*)bodyContact); // Middle
	b2Utils::AddRectangleFixture(body, 2, 21, 38, 32, 0, 0, 0)->SetUserData((void*)bodyContact); // Right Cover for not stuck on the walls, friction = 0

	// Foot sensors
	b2Utils::AddRectangleFixture(body, 3, 3, 26, 55, 0, 0, 0, true)->SetUserData((void*)footContact); // left feet
	b2Utils::AddRectangleFixture(body, 4, 2, 30, 57, 0, 0, 0, true)->SetUserData((void*)footContact); // mid feet
	b2Utils::AddRectangleFixture(body, 3, 3, 35, 55, 0, 0, 0, true)->SetUserData((void*)footContact); // right feet

	// Head
	b2Utils::AddCircleFixture(body, 8, 24, 17, 0, PLAYER_DENSITY / 2, 0)->SetUserData((void*)bodyContact); // head

	// Create Legs
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((worldPosition.x + 26) / BOX2D_SCALE, (worldPosition.y + 47) / BOX2D_SCALE);
	body_foot = world->CreateBody(&bodyDef);
	b2Utils::AddCircleFixture(body_foot, 6, 0, 0, 0, PLAYER_DENSITY / 2, PLAYER_FRICTION)->SetUserData((void*)bodyContact); // legs
	
	// Create Player-Foot Joint //
	b2RevoluteJointDef joint;
	joint.Initialize(body_foot, body, body_foot->GetWorldCenter());
	joint.enableMotor = true;
	joint.collideConnected = true;
	joint.maxMotorTorque = 100;
	joint.motorSpeed = 100;
	foot_joint = (b2RevoluteJoint*)world->CreateJoint(&joint);
	
	// Create Drop Effect
	dropDustEffect = new Effect("Dust");
}

Player::~Player() {
	delete spritesheet;
	delete walkAnimation;
	delete idleAnimation;

	delete dropDustEffect;
}

void Player::Render(sf::RenderWindow* window, Camera camera) {
	b2Vec2 position = body->GetPosition();

	currentAnimation->Render(window, sf::Vector2f(position.x * BOX2D_SCALE - camera.getPosition().x, position.y * BOX2D_SCALE - camera.getPosition().y), moveDirection == 1);

	//b2Utils::RenderFixtures(window, body, camera->getPosition(), true);
	b2Utils::RenderFixtures(window, body_foot, camera.getPosition(), false);

	sf::Vector2f effectPos;
	effectPos.x = body_foot->GetPosition().x * BOX2D_SCALE - camera.getPosition().x - 50 / 2;
	effectPos.y = body_foot->GetPosition().y * BOX2D_SCALE - camera.getPosition().y + body_foot->GetFixtureList()->GetShape()->m_radius * BOX2D_SCALE;
	dropDustEffect->Render(window, effectPos);
}

void Player::Update(int updateElapsed) {
	currentAnimation->Update(updateElapsed);

	if (isOnLadder > 0) {
		body->ApplyForceToCenter(b2Vec2(0, -10.0f * (body->GetMass() + body_foot->GetMass())), false);
	}

	dropDustEffect->Update(updateElapsed);
}

void Player::HandleInputs(int updateElapsed) {
	b2Vec2 vel = body->GetLinearVelocity();

	if (isOnLadder > 0) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			currentAnimation = walkAnimation;
			vel.y = -PLAYER_SPEED;
		}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			currentAnimation = walkAnimation;
			vel.y = PLAYER_SPEED;
		}else {
			currentAnimation = idleAnimation;
			vel.y = 0;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		currentAnimation = walkAnimation;
		moveDirection = -1;
		vel.x = -PLAYER_SPEED;
		foot_joint->SetMotorSpeed(100);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		currentAnimation = walkAnimation;
		moveDirection = 1;
		vel.x = PLAYER_SPEED;
		foot_joint->SetMotorSpeed(-100);
	}
	else {
		currentAnimation = idleAnimation;
		vel.x = 0;
		foot_joint->SetMotorSpeed(0);
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
				if (!isOnAir) {
					dropDustEffect->Start();
				}
				isOnAir = false;
			}
			else {
				isOnAir = true;
			}
		}
	}

	if (((ContactData*)interacted->GetUserData())->getDataType() == CONTACT_TYPE_SENSOR_INT) {
		if ((int)((ContactData*)interacted->GetUserData())->getData() == LADDER_SENSOR) {
			isOnLadder += isBegin ? 1 : -1;
		}
	}

}

sf::Vector2f Player::getPixPosition() {
	return sf::Vector2f(body->GetPosition().x * BOX2D_SCALE, body->GetPosition().y * BOX2D_SCALE);
}

b2Vec2 Player::getb2Position() {
	return body->GetPosition();
}