#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <Map.h>
#include <Player.h>
#include <Camera.h>

using namespace std;

#define CLOUDS_SPEED 100
#define PLAYER_CLOUD_MULT 10


class Background {
public:
	Background(Camera* map);
	~Background();

	void Update(int updateElapsed);
	void Render(sf::RenderWindow* window);

private:
	Camera * camera;

	sf::Texture* background_texture;

	sf::Vector2f clouds_position;
	sf::Sprite* clouds_sprite;
	sf::Texture* clouds_texture;

	sf::Vector2f hills_position;
	sf::Texture* hills_texture;
	sf::Sprite* hills_sprite;


};