#include <Mob.h>


Mob::Mob(b2World* world, Map* map, sf::Vector2f worldPosition) {
	this->map = map;

	std::string playerPath = "../../Resources/Mob/";

	// Create Animations
	spritesheet = new sf::Image();
	spritesheet->loadFromFile(playerPath + "Mob.png");

	walkAnimation = new Animation(spritesheet, "walk", 3, 100, 100, 7, 300, true);
	walkAnimation->Play();

	idleAnimation = new Animation(spritesheet, "idle", 3, 100, 100, 1, 680, false);
	idleAnimation->Play();
	
	currentAnimation = idleAnimation;
	
	// Create Physics
	b2BodyDef m_b2BodyDef;
	m_b2BodyDef.type = b2_dynamicBody;
	m_b2BodyDef.position.Set(worldPosition.x / BOX2D_SCALE, worldPosition.y / BOX2D_SCALE);
	body = world->CreateBody(&m_b2BodyDef);
	body->SetFixedRotation(true);

	// Create ContactData
	ContactData* bodyContact = new ContactData(CONTACT_TYPE_MOB_INSTANCE, this);
	ContactData* footContact = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)FOOT_SENSOR);
	body->SetUserData((void*)bodyContact);

	b2Utils::AddRectangleFixture(body, 2, 46, 41, 38, 0, 0, 0)->SetUserData((void*)bodyContact); // Left Cover for not stuck on the walls, friction = 0
	b2Utils::AddRectangleFixture(body, 15, 46, 43, 38, 0, PLAYER_DENSITY, PLAYER_FRICTION)->SetUserData((void*)bodyContact); // Middle
	b2Utils::AddRectangleFixture(body, 2, 46, 58, 38, 0, 0, 0)->SetUserData((void*)bodyContact); // Right Cover for not stuck on the walls, friction = 0

	b2Utils::AddRectangleFixture(body, 4, 4, 42, 87, 0, 0, 0, true)->SetUserData((void*)footContact); // left feet
	b2Utils::AddRectangleFixture(body, 4, 4, 48, 88, 0, 0, 0, true)->SetUserData((void*)footContact); // mid feet
	b2Utils::AddRectangleFixture(body, 4, 4, 54, 87, 0, 0, 0, true)->SetUserData((void*)footContact); // right feet

	b2Utils::AddCircleFixture(body, 9, 42, 69, 0, PLAYER_DENSITY / 2, PLAYER_FRICTION)->SetUserData((void*)bodyContact); // legs
	b2Utils::AddCircleFixture(body, 10, 41, 18, 0, PLAYER_DENSITY / 2, 0)->SetUserData((void*)bodyContact); // head
}

Mob::~Mob() {
	delete spritesheet;
	delete walkAnimation;
	delete idleAnimation;
}

void Mob::Render(sf::RenderWindow* window) {
	b2Vec2 position = body->GetPosition();

	currentAnimation->Render(window, sf::Vector2f(position.x * BOX2D_SCALE - map->m_offset.x, position.y * BOX2D_SCALE - map->m_offset.y), moveDirection == 1);

	b2Utils::RenderFixtures(window, body, map->m_offset);
	
}

void Mob::Update(int updateElapsed) {
	b2Vec2 vel = body->GetLinearVelocity();

	if (vel.x == 0) {
		currentAnimation = idleAnimation;
	}
	else if (vel.x < 0) {
		currentAnimation = walkAnimation;
		moveDirection = -1;
	}
	else if (vel.x > 0) {
		currentAnimation = walkAnimation;
		moveDirection = 1;
	}

	currentAnimation->Update(updateElapsed);
}

void Mob::HandleCollision(b2Fixture* self, b2Fixture* interacted, bool isBegin) {

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