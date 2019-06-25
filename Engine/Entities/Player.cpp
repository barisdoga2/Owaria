#include <Player.h>
#include <algorithm>



Player::Player(b2World* world, Map* map, sf::Vector2f worldPosition) {
	this->map = map;

	std::string playerPath = "../../Resources/Player/";

	// Create Animations
	bodySpriteSheet = new sf::Texture();
	bodySpriteSheet->loadFromFile(playerPath + "Body.png");

	walkAnimation = new Animation("walk", sf::Vector2i(0, 9 * 64), sf::Vector2i(64, 64), 9, 300, true);
	climbUpAnimation = new Animation("climbUp", sf::Vector2i(0, 8 * 64), sf::Vector2i(64, 64), 9, 300, true);
	climbDownAnimation = new Animation("climbDown", sf::Vector2i(0, 10 * 64), sf::Vector2i(64, 64), 9, 300, true);
	idleAnimation = new Animation("idle", sf::Vector2i(0, 1 * 64), sf::Vector2i(64, 64), 2, 680, true);
	slashAnimation = new Animation("slash", sf::Vector2i(0, 13 * 64), sf::Vector2i(64, 64), 6, 50, false);

	currentBodyAnimation = idleAnimation;

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
	joint.maxMotorTorque = PLAYER_TORQUE;
	joint.motorSpeed = PLAYER_SPEED;
	foot_joint = (b2RevoluteJoint*)world->CreateJoint(&joint);

	// Weapons
	daggerWeapon = new Weapon("dagger", body, playerPath + "Dagger.png", slashAnimation);

}

Player::~Player() {
	delete bodySpriteSheet;
	delete walkAnimation;
	delete idleAnimation;
	delete climbUpAnimation;
	delete climbDownAnimation;
	delete slashAnimation;
	delete daggerWeapon;
}

void Player::Render(sf::RenderWindow* window, Camera camera) {
	b2Vec2 position = body->GetPosition();

	// Render Body
	currentBodyAnimation->Render(window, bodySpriteSheet, sf::Vector2f(position.x * BOX2D_SCALE - camera.getPosition().x, position.y * BOX2D_SCALE - camera.getPosition().y), moveDirection.x == RIGHT_DIRECTION);
	// Render Dagger
	if (daggerWeapon != nullptr)
		currentBodyAnimation->Render(window, daggerWeapon->GetSpriteSheet(), sf::Vector2f(position.x * BOX2D_SCALE - camera.getPosition().x, position.y * BOX2D_SCALE - camera.getPosition().y), moveDirection.x == RIGHT_DIRECTION);
}

void Player::Update(int updateElapsed) {
	currentBodyAnimation->Update(updateElapsed);

	// If on Ladder Apply Negative Force to Prevent falling
	if (isOnLadder) {
		body_foot->ApplyForceToCenter(b2Vec2(0, -WORLD_GRAVITY * (body->GetMass() + body_foot->GetMass())), false);
	}

	// Update Weapon
	if (daggerWeapon != nullptr)
		daggerWeapon->Update(updateElapsed, moveDirection.x == RIGHT_DIRECTION);
}

void Player::HandleInputs(int updateElapsed) {
	b2Vec2 vel = body->GetLinearVelocity();

	// Handle Ground Movements
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		isWalking = true;
		moveDirection.x = LEFT_DIRECTION;
		vel.x = -PLAYER_SPEED;
		foot_joint->SetMotorSpeed(PLAYER_SPEED);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		isWalking = true;
		moveDirection.x = RIGHT_DIRECTION;
		vel.x = PLAYER_SPEED;
		foot_joint->SetMotorSpeed(-PLAYER_SPEED);
	}
	else {
		isWalking = false;
		vel.x = 0;
		foot_joint->SetMotorSpeed(0);
	}

	// Handle Ladder
	if (isOnLadder) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			moveDirection.y = TOP_DIRECTION;
			vel.y = -PLAYER_SPEED;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			moveDirection.y = DOWN_DIRECTION;
			vel.y = PLAYER_SPEED;
		}
		else {
			moveDirection.y = NONE_DIRECTION;
			vel.y = 0;
		}
	}
	body->SetLinearVelocity(vel);

	// Handle Jumping
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !isOnAir) {
		body->ApplyForceToCenter(b2Vec2(0, -WORLD_GRAVITY * (body->GetMass() + body_foot->GetMass()) * 65), false);
		isOnAir = true;
	}
	
	// Handle Attacking
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && daggerWeapon != nullptr)
		daggerWeapon->StartAttack();

	// Basic Animation "State Machine"
	currentBodyAnimation = idleAnimation;

	if (isWalking) 
		currentBodyAnimation = walkAnimation;

	if (isOnLadder) 
		if (moveDirection.y > 0)
			currentBodyAnimation = climbUpAnimation;
		else if(moveDirection.y < 0)
			currentBodyAnimation = climbDownAnimation;

	if (daggerWeapon != nullptr && daggerWeapon->IsAttacking())
		currentBodyAnimation = slashAnimation;

	if(currentBodyAnimation->isFinished())
		currentBodyAnimation->Play();
}

void Player::HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin) {
	
	if (((ContactData*)self->GetUserData())->getDataType() == CONTACT_TYPE_SENSOR_INT && ((ContactData*)interacted->GetUserData())->getDataType() == CONTACT_TYPE_SENSOR_INT) {
		if ((int)((ContactData*)self->GetUserData())->getData() == FOOT_SENSOR && (int)((ContactData*)interacted->GetUserData())->getData() == MAP_SENSOR) {
			numFootContacts += isBegin ? 1 : -1;
			isOnAir = numFootContacts == 0;
		}

		if ((int)((ContactData*)self->GetUserData())->getData() == FOOT_SENSOR && (int)((ContactData*)interacted->GetUserData())->getData() == LADDER_SENSOR) {
			numLadderContacts += isBegin ? 1 : -1;
			isOnLadder = numLadderContacts > 0;
		}
	}

	if (((ContactData*)self->GetUserData())->getDataType() == CONTACT_TYPE_SENSOR_INT && ((ContactData*)interacted->GetUserData())->getDataType() != CONTACT_TYPE_PLAYER_INSTANCE) {
		if ((int)((ContactData*)self->GetUserData())->getData() == WEAPON_SENSOR) {
			cout << "Contact Weapon" << endl;
		}
	}
}

sf::Vector2f Player::getPixPosition() {
	return sf::Vector2f(body->GetPosition().x * BOX2D_SCALE, body->GetPosition().y * BOX2D_SCALE);
}

b2Vec2 Player::getb2Position() {
	return body->GetPosition();
}