#include <Camera.h>

Camera::Camera() {
	this->m_map = nullptr;
	this->m_player = nullptr;
}

Camera::Camera(Map* p_map, Player* p_player) {
	this->m_map = p_map;
	this->m_player = p_player;
}

void Camera::Update() {
	if (m_player != nullptr && m_map != nullptr) {
		b2Vec2 position = m_player->body->GetPosition();

		m_map->m_offset.x = position.x * BOX2D_SCALE - SCREEN_WIDTH / 2.0f;
		if (m_map->m_offset.x < 0)
			m_map->m_offset.x = 0;
		else if (m_map->m_offset.x > m_map->getMapWidth() * 16 - SCREEN_WIDTH)
			m_map->m_offset.x = (float)m_map->getMapWidth() * 16 - SCREEN_WIDTH;

		m_map->m_offset.y = position.y * BOX2D_SCALE - SCREEN_HEIGHT / 2.0f;
		if (m_map->m_offset.y < 0)
			m_map->m_offset.y = 0;
		else if (m_map->m_offset.y > m_map->getMapHeight() * 16 - SCREEN_HEIGHT)
			m_map->m_offset.y = (float)m_map->getMapHeight() * 16 - SCREEN_HEIGHT;
	}
}

void Camera::SetMap(Map* p_map) {
	this->m_map = p_map;
}

void Camera::SetTarget(Player* p_player) {
	this->m_player = p_player;
}