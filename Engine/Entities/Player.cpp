#include <Player.h>
#include <algorithm>

#define SPEED 1
#define TORQUE 10
#define LEFT_DIR -1
#define RIGHT_DIR 1
#define TOP_DIR 1
#define DOWN_DIR -1
#define NONE_DIR 0



Player::Player(b2World* world, Map* map, sf::Vector2f worldPosition) {
	this->map = map;

	std::string playerPath = "../../Resources/Player/";

	// Create Animations
	spritesheet = new sf::Texture();
	spritesheet2 = new sf::Texture();
	spritesheet->loadFromFile(playerPath + "Body.png");
	spritesheet2->loadFromFile(playerPath + "Dagger.png");

	walkAnimation = new Animation("walk", 0, 9 * 64, 64, 64, 9, 300, true);
	walkAnimation->Play();

	climbUpAnimation = new Animation("climbUp", 0, 8 * 64, 64, 64, 9, 300, true);
	climbUpAnimation->Play();

	climbDownAnimation = new Animation("climbDown", 0, 10 * 64, 64, 64, 9, 300, true);
	climbDownAnimation->Play();

	idleAnimation = new Animation("idle", 0, 1 * 64, 64, 64, 2, 680, true);
	idleAnimation->Play();

	slashAnimation = new Animation("slash", 0, 13 * 64, 64, 64, 6, 50, false);
	slashAnimation->Play();

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
	joint.maxMotorTorque = TORQUE;
	joint.motorSpeed = SPEED;
	foot_joint = (b2RevoluteJoint*)world->CreateJoint(&joint);

	// Weapons
	weaponData = new WeaponData("dagger", *slashAnimation, *spritesheet2); // Extract Weapon Data from Animation Frames and Sprite Sheet
	dagger = nullptr;
}

Player::~Player() {
	delete spritesheet;
	delete spritesheet2;
	delete walkAnimation;
	delete idleAnimation;
	delete slashAnimation;
	delete climbUpAnimation;
	delete climbDownAnimation;
	delete weaponData;
}

void Player::Render(sf::RenderWindow* window, Camera camera) {
	b2Vec2 position = body->GetPosition();

	// Render Body
	currentAnimation->Render(window, spritesheet, sf::Vector2f(position.x * BOX2D_SCALE - camera.getPosition().x, position.y * BOX2D_SCALE - camera.getPosition().y), moveDirection.x == RIGHT_DIR);
	// Render Dagger
	currentAnimation->Render(window, spritesheet2, sf::Vector2f(position.x * BOX2D_SCALE - camera.getPosition().x, position.y * BOX2D_SCALE - camera.getPosition().y), moveDirection.x == RIGHT_DIR);
}

void Player::Update(int updateElapsed) {
	currentAnimation->Update(updateElapsed);

	// If on Ladder Apply Negative Force to Prevent falling
	if (isOnLadder) {
		body_foot->ApplyForceToCenter(b2Vec2(0, -WORLD_GRAVITY * (body->GetMass() + body_foot->GetMass())), false);
	}

	// Apply attack frames to physics
	if (currentAnimation->GetName().compare("slash") == 0) {
		if (dagger != nullptr)
			body->DestroyFixture(dagger);

		ContactData* daggerContact = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)WEAPON_SENSOR);
		vector<sf::Vector3i> points = weaponData->frame_points.at(currentAnimation->GetCurrentFrame());
		vector<sf::Vector2i> points2i;
		for (sf::Vector3i v : points)
			points2i.push_back(sf::Vector2i(v.x, v.y));
		dagger = b2Utils::AddChainLoopFixture(body, points2i, 0, 0, 0, true);
		dagger->SetUserData((void*)daggerContact);

	}
	else {
		if (dagger != nullptr) {
			body->DestroyFixture(dagger);
			dagger = nullptr;
		}
			
	}
}

void Player::HandleInputs(int updateElapsed) {
	b2Vec2 vel = body->GetLinearVelocity();

	// Handle Ground Movements
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		isWalking = true;
		moveDirection.x = LEFT_DIR;
		vel.x = -PLAYER_SPEED;
		foot_joint->SetMotorSpeed(SPEED);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		isWalking = true;
		moveDirection.x = RIGHT_DIR;
		vel.x = PLAYER_SPEED;
		foot_joint->SetMotorSpeed(-SPEED);
	}
	else {
		isWalking = false;
		vel.x = 0;
		foot_joint->SetMotorSpeed(0);
	}

	// Handle Ladder
	if (isOnLadder) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			moveDirection.y = TOP_DIR;
			vel.y = -PLAYER_SPEED;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			moveDirection.y = DOWN_DIR;
			vel.y = PLAYER_SPEED;
		}
		else {
			moveDirection.y = NONE_DIR;
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
	static int countDown;
	countDown -= updateElapsed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !isAttacking && countDown <= 0) {
		countDown = 1000;
		isAttacking = true;
	}
	else {
		if (slashAnimation->isFinished())
			isAttacking = false;
	}

	// Basic Animation "State Machine"
	currentAnimation = idleAnimation;

	if (isWalking) 
		currentAnimation = walkAnimation;

	if (isOnLadder) 
		if (moveDirection.y > 0)
			currentAnimation = climbUpAnimation;
		else if(moveDirection.y < 0)
			currentAnimation = climbDownAnimation;

	if (isAttacking)
		currentAnimation = slashAnimation;

	if(currentAnimation->isFinished())
		currentAnimation->Play();
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
	static int i = 0;
	if (((ContactData*)self->GetUserData())->getDataType() == CONTACT_TYPE_SENSOR_INT && ((ContactData*)interacted->GetUserData())->getDataType() != CONTACT_TYPE_PLAYER_INSTANCE) {
		if ((int)((ContactData*)self->GetUserData())->getData() == WEAPON_SENSOR) {
			cout << "Contact Weapon: " << i++ << endl;
		}
	}
}

sf::Vector2f Player::getPixPosition() {
	return sf::Vector2f(body->GetPosition().x * BOX2D_SCALE, body->GetPosition().y * BOX2D_SCALE);
}

b2Vec2 Player::getb2Position() {
	return body->GetPosition();
}