#pragma once

#include <Player.h>
#include <Map.h>


class Camera {

public:
	Camera();
	Camera(Map* p_map, Player* p_player);

	void Update(int updateElapsed);
	void SetMap(Map* p_map);
	void SetTarget(Player* p_player);
	Player* GetTarget();
	Map* GetMap();
	sf::Vector2f isOffsetsLocked();

private:
	Player * m_player;
	Map* m_map;
	sf::Vector2f position;

	void calculateMapOffset();

};
