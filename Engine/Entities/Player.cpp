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

	// Process
	vector<vector<sf::Vector3i>> frame_points;
	int xx = 0;
	int yy = 13 * 64;
	int ww = 64;
	int hh = 64;
	int ll = 6;

	const sf::Image im = spritesheet2->copyToImage();
	const int imWidth = im.getSize().x;
	const sf::Uint8* pixels = im.getPixelsPtr();
	for (int l = 0; l < ll; l++) {
		vector<sf::Vector3i> points;
		for (int y = yy; y < yy + hh; y++) {
			for (int x = xx + l * ww; x < xx + ww + l * ww; x++) {
				const sf::Uint8* offset = pixels + (x + y * imWidth) * 4;
				int r = (int)*(offset + 0);
				int g = (int)*(offset + 1);
				int b = (int)*(offset + 2);
				int a = (int)*(offset + 3);
				if (r == 127 && g == 1 && b == 127) 
					points.push_back(sf::Vector3i(x % 64, y % 64, a));
			}
		}
		sort(points.begin(), points.end(), [](const sf::Vector3i& lhs, const sf::Vector3i& rhs) {
			return lhs.z < rhs.z;
		});
		frame_points.push_back(points);
	}

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
	ContactData* weaponContact = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)WEAPON_SENSOR);
	body->SetUserData((void*)bodyContact);

	// Body
	b2Utils::AddRectangleFixture(body, 2, 21, 24, 32, 0, 0, 0)->SetUserData((void*)bodyContact); // Left Cover for not stuck on the walls, friction = 0
	b2Utils::AddRectangleFixture(body, 12, 21, 26, 32, 0, PLAYER_DENSITY, PLAYER_FRICTION)->SetUserData((void*)bodyContact); // Middle
	b2Utils::AddRectangleFixture(body, 2, 21, 38, 32, 0, 0, 0)->SetUserData((void*)bodyContact); // Right Cover for not stuck on the walls, friction = 0

	// Foot sensors
	b2Utils::AddRectangleFixture(body, 3, 3, 26, 55, 0, 0, 0, true)->SetUserData((void*)footContact); // left feet
	b2Utils::AddRectangleFixture(body, 4, 2, 30, 57, 0, 0, 0, true)->SetUserData((void*)footContact); // mid feet
	b2Utils::AddRectangleFixture(body, 3, 3, 35, 55, 0, 0, 0, true)->SetUserData((void*)footContact); // right feet

	// Weapon Sensors
	vector<sf::Vector2i> t_vertices;
	t_vertices.push_back(sf::Vector2i(2, 38));
	t_vertices.push_back(sf::Vector2i(5, 35));
	t_vertices.push_back(sf::Vector2i(10, 34));
	t_vertices.push_back(sf::Vector2i(12, 38));
	t_vertices.push_back(sf::Vector2i(7, 39));
	b2Fixture* dagger = b2Utils::AddChainLoopFixture(body, t_vertices, 0, 0, 0, true);
	dagger->SetUserData((void*)weaponContact);

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

}

Player::~Player() {
	delete spritesheet;
	delete spritesheet2;
	delete walkAnimation;
	delete idleAnimation;
	delete slashAnimation;
	delete climbUpAnimation;
	delete climbDownAnimation;
}

void Player::Render(sf::RenderWindow* window, Camera camera) {
	b2Vec2 position = body->GetPosition();

	currentAnimation->Render(window, spritesheet, sf::Vector2f(position.x * BOX2D_SCALE - camera.getPosition().x, position.y * BOX2D_SCALE - camera.getPosition().y), moveDirection.x == RIGHT_DIR);
	currentAnimation->Render(window, spritesheet2, sf::Vector2f(position.x * BOX2D_SCALE - camera.getPosition().x, position.y * BOX2D_SCALE - camera.getPosition().y), moveDirection.x == RIGHT_DIR);
}

void Player::Update(int updateElapsed) {
	currentAnimation->Update(updateElapsed);

	if (isOnLadder) {
		body_foot->ApplyForceToCenter(b2Vec2(0, -WORLD_GRAVITY * (body->GetMass() + body_foot->GetMass())), false);
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
	
	if (((ContactData*)self->GetUserData())->getDataType() == CONTACT_TYPE_SENSOR_INT) {
		if ((int)((ContactData*)self->GetUserData())->getData() == FOOT_SENSOR && (int)((ContactData*)interacted->GetUserData())->getData() == MAP_SENSOR) {
			numFootContacts += isBegin ? 1 : -1;
			isOnAir = numFootContacts == 0;
		}
	}

	if (((ContactData*)interacted->GetUserData())->getDataType() == CONTACT_TYPE_SENSOR_INT) {
		if ((int)((ContactData*)interacted->GetUserData())->getData() == LADDER_SENSOR && (int)((ContactData*)self->GetUserData())->getData() == FOOT_SENSOR) {
			numLadderContacts += isBegin ? 1 : -1;
			isOnLadder = numLadderContacts > 0;
		}
	}

	if (((ContactData*)self->GetUserData())->getDataType() == CONTACT_TYPE_SENSOR_INT) {
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