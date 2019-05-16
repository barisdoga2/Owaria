#include <GameObject.h>



GameObject::GameObject(GameObjectData* data, sf::Vector2i tilemapPos) {
	this->data = data;
	this->tilemapPos = tilemapPos;
}

GameObject::~GameObject() {
	
}

sf::Vector2i GameObject::getTilemapPos() {
	return this->tilemapPos;
}

GameObjectData* GameObject::getGameObjectData() {
	return this->data;
}