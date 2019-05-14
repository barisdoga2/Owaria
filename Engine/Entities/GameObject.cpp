#include <GameObject.h>



GameObject::GameObject(game_obj_data* data, sf::Vector2i tilemapPos) {
	this->data = data;
	this->tilemapPos = tilemapPos;
}

GameObject::~GameObject() {
	
}

sf::Vector2i GameObject::getTilemapPos() {
	return this->tilemapPos;
}

game_obj_data* GameObject::getGameObjectData() {
	return this->data;
}