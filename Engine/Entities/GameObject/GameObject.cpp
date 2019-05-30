#include <GameObject.h>
#include <Settings.h>
#include <ContactData.h>
#include <b2Utils.h>



GameObject::GameObject(GameObjectData* data, b2World* world, sf::Vector2i tilemapPos) {
	this->data = data;
	this->tilemapPos = tilemapPos;


	if (data->hasPhysicsBody) {
		b2BodyDef bodyDef;
		bodyDef.type = data->isStatic == 1 ? b2_staticBody : b2_dynamicBody;
		bodyDef.position = b2Vec2((tilemapPos.x * 16) / BOX2D_SCALE, (tilemapPos.y * 16) / BOX2D_SCALE);

		objectBody = world->CreateBody(&bodyDef);

		if (data->contactDataType == CONTACT_TYPE_SENSOR_INT) {
			if (data->contactDataObject == LADDER_SENSOR) {
				ContactData* cont = new ContactData(CONTACT_TYPE_SENSOR_INT, (void*)LADDER_SENSOR);
				objectBody->SetUserData(cont);
				b2Utils::AddRectangleFixture(objectBody, (int)data->ObjectSet->getTileset()->getTilePixelSize().x, (int)data->ObjectSet->getTileset()->getTilePixelSize().y, 0, 0, 0, 10.0f, 0.5f, true)->SetUserData(cont);
			}
		}

	}
}

GameObject::~GameObject() {
	
}

sf::Vector2i GameObject::getTilemapPos() {
	return this->tilemapPos;
}

GameObjectData* GameObject::getGameObjectData() {
	return this->data;
}