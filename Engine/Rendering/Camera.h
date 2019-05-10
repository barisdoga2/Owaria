#pragma once

#include <Player.h>
#include <Map.h>

class Camera {

public:
	Camera();
	Camera(Map* p_map, Player* p_player);

	void Update();
	void SetMap(Map* p_map);
	void SetTarget(Player* p_player);

private:
	Player * m_player;
	Map* m_map;

};