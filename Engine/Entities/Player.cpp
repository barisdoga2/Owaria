#include <Player.h>
#include <algorithm>



Player::Player(b2World* world, Map* map, sf::Vector2f worldPosition, int sex) {
	this->map = map;
	this->sex = sex;

	// Load Player Base Sprite
	bodySpriteSheet = new sf::Texture();
	if(sex == MALE_SEX)
		bodySpriteSheet->loadFromFile("../../Resources/Player/BodyMale.png");
	else if (sex == FEMALE_SEX)
		bodySpriteSheet->loadFromFile("../../Resources/Player/BodyFemale.png");
	
	// Create All Animations
	CreateAnimations();

	// Create Physics
	CreatePhysics(world, worldPosition);
}

Player::~Player() {
	delete bodySpriteSheet;
	
	for (int i = 0; i < WEARABLE_COUNT; i++)
		if (items[i] != nullptr)
			delete items[i];

	for (Animation* a : animations)
		delete a;
}

bool Player::WearItem(ItemAsset* itemAsset) {
	switch (itemAsset->getType())
	{
	case WEARABLE_WEAPON:
		items[itemAsset->getType()] = new MeleeWeapon(itemAsset, GetAnimation(itemAsset->getAnimationAsset()->getName()));
		break;

	case WEARABLE_SHIELD:
		items[itemAsset->getType()] = new MeleeWeapon(itemAsset, GetAnimation(itemAsset->getAnimationAsset()->getName()));
		break;

	default:
		items[itemAsset->getType()] = new Armor(itemAsset);
		break;
	}

	return true;
}

bool Player::UnWearItem(ItemAsset* itemAsset) {
	items[itemAsset->getType()] = nullptr;

	return true;
}

void Player::Render(sf::RenderWindow* window, Camera camera) {
	sf::Vector2f renderPos = sf::Vector2f(body->GetPosition().x * BOX2D_SCALE - camera.getPosition().x, body->GetPosition().y * BOX2D_SCALE - camera.getPosition().y);
	bool yMirror = moveDirection.x == RIGHT_DIRECTION;

	// Render Body
	currentBodyAnimation->Render(window, bodySpriteSheet, renderPos, yMirror);
	
	// Render Wearables
	for(int i = 0 ; i < WEARABLE_COUNT ; i++)
		if (items[i] != nullptr) currentBodyAnimation->Render(window, items[i]->GetItemAsset()->getSpriteSheet(sex),	renderPos, yMirror);
	
}

void Player::Update(int updateElapsed) {
	currentBodyAnimation->Update(updateElapsed);

	// If on Ladder Apply Negative Force to Prevent falling
	if (isOnLadder)
		body_foot->ApplyForceToCenter(b2Vec2(0, -WORLD_GRAVITY * (body->GetMass() + body_foot->GetMass())), false);

	// Update Weapon
	if (items[WEARABLE_WEAPON] != nullptr) ((MeleeWeapon*)items[WEARABLE_WEAPON])->Update(updateElapsed, body, sex, moveDirection.x == RIGHT_DIRECTION);
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && items[WEARABLE_WEAPON] != nullptr)
		((MeleeWeapon*)items[WEARABLE_WEAPON])->StartAttack();

	// Basic Animation "State Machine"
	currentBodyAnimation = GetAnimation(IDLE);
	if (isWalking)
		currentBodyAnimation = GetAnimation(WALK);

	if (isOnLadder)
		if (moveDirection.y > 0)
			currentBodyAnimation = GetAnimation(CLIMB_UP);
		else if (moveDirection.y < 0)
			currentBodyAnimation = GetAnimation(CLIMB_DOWN);

	if (items[WEARABLE_WEAPON] != nullptr && ((MeleeWeapon*)items[WEARABLE_WEAPON])->IsAttacking())
		currentBodyAnimation = ((MeleeWeapon*)items[WEARABLE_WEAPON])->GetAnimation();

	if (currentBodyAnimation->isFinished())
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

void Player::CreatePhysics(b2World* world, sf::Vector2f worldPosition) {
	// Create Main Body
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
}

void Player::LoadAssets() {
	// Load All Assets Related with Player
	XMLDocument tDoc;
	tDoc.LoadFile("../../Resources/Player/AnimationSet.xml");
	XMLUtils::LoadAnimationSet(tDoc.FirstChildElement("AnimationSet"));
}

void Player::CreateAnimations() {
	for (AnimationAsset* a : AssetStore::GetAnimationSet("player")->getAllAnimationAssets())
		animations.push_back(new Animation(a));
	currentBodyAnimation = GetAnimation(IDLE);
}

Animation* Player::GetAnimation(string animationAssetName) {
	for (Animation* a : animations)
		if (a->getAnimationAssetName().compare(animationAssetName) == 0)
			return a;

	return nullptr;
}

sf::Vector2f Player::getPixPosition() {
	return sf::Vector2f(body->GetPosition().x * BOX2D_SCALE, body->GetPosition().y * BOX2D_SCALE);
}

b2Vec2 Player::getb2Position() {
	return body->GetPosition();
}

b2Body* Player::getBody() {
	return body;
}
