#pragma once

#include <SFML/Graphics.hpp>

class Player;
#include <Player.h>

class MyContactListener : public b2ContactListener
{

public:
	MyContactListener(Player* p);
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

private:
	Player* p;
	
};