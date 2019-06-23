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

	if (m_map != nullptr) {
		if (!isFreeRoam && m_player != nullptr) {
			calculateTarget();
			position.x += (target.x - position.x) * CAMERA_LERP * updateElapsed;
			position.y += (target.y - position.y) * CAMERA_LERP * updateElapsed;
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				position.x -= CAMERA_FREE_ROAM_SPEED * updateElapsed;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				position.x += CAMERA_FREE_ROAM_SPEED * updateElapsed;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				position.y -= CAMERA_FREE_ROAM_SPEED * updateElapsed;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				position.y += CAMERA_FREE_ROAM_SPEED * updateElapsed;

			position.x = position.x < 0 ? 0 : position.x;
			position.x = position.x > m_map->getMapSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH ? m_map->getMapSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH : position.x;
			position.y = position.y < 0 ? 0 : position.y;
			position.y = position.y > m_map->getMapSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT ? m_map->getMapSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT : position.y;
		}
	}
			
	target.x = position.x;
	target.y = position.y;
	
}

void Camera::calculateTarget() {
	b2Vec2 playerb2Pos = m_player->getb2Position();

	target.x = playerb2Pos.x * BOX2D_SCALE - SCREEN_WIDTH / 2.0f;
	if (target.x < 0)
		target.x = 0;
	else if (target.x > m_map->getMapSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH)
		target.x = (float)m_map->getMapSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH;

	target.y = playerb2Pos.y * BOX2D_SCALE - SCREEN_HEIGHT / 1.5f;
	if (target.y < 0)
		target.y = 0;
	else if (target.y > m_map->getMapSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT)
		target.y = (float)m_map->getMapSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT;
}

Map* Camera::GetMap() {
	return this->m_map;
}

Player* Camera::GetTarget() {
	return this->m_player;
}

sf::Vector2f Camera::isOffsetsLocked() {
	return sf::Vector2f(target.x == 0 || (target.x == (float)m_map->getMapSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH), target.y == 0 || (target.y == (float)m_map->getMapSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT));
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

void Camera::SetFreeRoam(bool isFreeRoam) {
	this->isFreeRoam = isFreeRoam;
}

bool Camera::isCameraFreeRoam() {
	return isFreeRoam;
}