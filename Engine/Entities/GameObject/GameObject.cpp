#include <GameObject.h>
#include <Settings.h>
#include <ContactData.h>
#include <b2Utils.h>



GameObject::GameObject(ObjectAsset* data, b2World* world, sf::Vector2i tilemapPos) {
	this->data = data;
	this->tilemapPos = tilemapPos;


	if (data->isHasPhysics()) {
		b2BodyDef bodyDef;
		bodyDef.type = data->isHasStaticPhysics() == 1 ? b2_staticBody : b2_dynamicBody;
		bodyDef.position = b2Vec2((tilemapPos.x * 16) / BOX2D_SCALE, (tilemapPos.y * 16) / BOX2D_SCALE);

		objectBody = world->CreateBody(&bodyDef);

		if (data->getContactDataType() == CONTACT_TYPE_SENSOR_INT) {
			if (data->getContactDataObject() == LADDER_SENSOR) {
				ContactData* cont = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)LADDER_SENSOR);
				objectBody->SetUserData(cont);
				b2Utils::AddRectangleFixture(objectBody, (int)data->getObjectSet()->getTileset()->getTilePixelSize().x, (int)data->getObjectSet()->getTileset()->getTilePixelSize().y, 0, 0, 0, 10.0f, 0.5f, true)->SetUserData(cont);
			}
		}

	}
}

GameObject::~GameObject() {
	
}

sf::Vector2i GameObject::getTilemapPos() {
	return this->tilemapPos;
}

ObjectAsset* GameObject::getGameObjectData() {
	return this->data;
}