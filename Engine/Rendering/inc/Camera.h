#pragma once

class Player;
#include <Player.h>
#include <Map.h>


class Camera {

public:
	Camera();
	Camera(Map* p_map, Player* p_player);
	void Update(int updateElapsed);

	void SetMap(Map* p_map);
	void SetTarget(Player* p_player);
	void SetFreeRoam(bool isFreeRoam);
	bool isCameraFreeRoam();
	Player* GetTarget();
	Map* GetMap();

	sf::Vector2f isOffsetsLocked();
	sf::Vector2f getPosition();

private:
	Player * m_player;
	Map* m_map;
	sf::Vector2f position;
	sf::Vector2f target;
	bool isFreeRoam = false;

	void calculateTarget();

};
