#include <Camera.h>

Camera::Camera() {
	this->m_map = nullptr;
	this->m_player = nullptr;
}

Camera::Camera(Map* p_map, Player* p_player) {
	this->m_map = p_map;
	SetTarget(p_player);
}

void Camera::Update(int updateElapsed) {
	if (m_player != nullptr && m_map != nullptr) {
		
		sf::Vector2f playerPixPos = m_player->getPixPosition();

		calculateTarget();
		position.x += (target.x - position.x) * CAMERA_LERP * updateElapsed;
		position.y += (target.y - position.y) * CAMERA_LERP * updateElapsed;

		target.x = position.x;
		target.y = position.y;
	}

}

void Camera::calculateTarget() {
	b2Vec2 playerb2Pos = m_player->getb2Position();

	target.x = playerb2Pos.x * BOX2D_SCALE - SCREEN_WIDTH / 2.0f;
	if (target.x < 0)
		target.x = 0;
	else if (target.x > m_map->getMapTileSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH)
		target.x = (float)m_map->getMapTileSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH;

	target.y = playerb2Pos.y * BOX2D_SCALE - SCREEN_HEIGHT / 2.0f;
	if (target.y < 0)
		target.y = 0;
	else if (target.y > m_map->getMapTileSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT)
		target.y = (float)m_map->getMapTileSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT;
}

Map* Camera::GetMap() {
	return this->m_map;
}

Player* Camera::GetTarget() {
	return this->m_player;
}

sf::Vector2f Camera::isOffsetsLocked() {
	return sf::Vector2f(target.x == 0 || (target.x == (float)m_map->getMapTileSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH), target.y == 0 || (target.y == (float)m_map->getMapTileSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT));
}

void Camera::SetMap(Map* p_map) {
	this->m_map = p_map;
}

void Camera::SetTarget(Player* p_player) {
	this->m_player = p_player;

	calculateTarget();
	position = sf::Vector2f(target.x, target.y);

	this->m_player = p_player;
}

sf::Vector2f Camera::getPosition() {
	return position;
}