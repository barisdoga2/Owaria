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
		calculateMapOffset();
		
		sf::Vector2f playerPixPos = m_player->getPixPosition();

		position.x += (m_map->m_offset.x - position.x) * CAMERA_LERP * updateElapsed;
		position.y += (m_map->m_offset.y - position.y) * CAMERA_LERP * updateElapsed;

		m_map->m_offset.x = position.x;
		m_map->m_offset.y = position.y;
	}

}

void Camera::calculateMapOffset() {
	b2Vec2 playerb2Pos = m_player->getb2Position();

	m_map->m_offset.x = playerb2Pos.x * BOX2D_SCALE - SCREEN_WIDTH / 2.0f;
	if (m_map->m_offset.x < 0)
		m_map->m_offset.x = 0;
	else if (m_map->m_offset.x > m_map->getMapTileSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH)
		m_map->m_offset.x = (float)m_map->getMapTileSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH;

	m_map->m_offset.y = playerb2Pos.y * BOX2D_SCALE - SCREEN_HEIGHT / 2.0f;
	if (m_map->m_offset.y < 0)
		m_map->m_offset.y = 0;
	else if (m_map->m_offset.y > m_map->getMapTileSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT)
		m_map->m_offset.y = (float)m_map->getMapTileSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT;
}

Map* Camera::GetMap() {
	return this->m_map;
}

Player* Camera::GetTarget() {
	return this->m_player;
}

sf::Vector2f Camera::isOffsetsLocked() {
	return sf::Vector2f(m_map->m_offset.x == 0 || (m_map->m_offset.x == (float)m_map->getMapTileSize().x * m_map->getTileset()->getTilePixelSize().x - SCREEN_WIDTH), m_map->m_offset.y == 0 || (m_map->m_offset.y == (float)m_map->getMapTileSize().y * m_map->getTileset()->getTilePixelSize().y - SCREEN_HEIGHT));
}

void Camera::SetMap(Map* p_map) {
	this->m_map = p_map;
}

void Camera::SetTarget(Player* p_player) {
	this->m_player = p_player;

	calculateMapOffset();
	position = sf::Vector2f(m_map->m_offset.x, m_map->m_offset.y);

	this->m_player = p_player;
}