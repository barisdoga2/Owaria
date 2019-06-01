#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <Map.h>
#include <Player.h>
#include <Camera.h>

using namespace std;

#define CLOUDS_SPEED 50
#define PLAYER_CLOUD_MULT 10
#define SLOW_CLOUD_DIV 3


class Background {
public:
	Background();
	~Background();

	void Update(int updateElapsed, Camera* camera);
	void Render(sf::RenderWindow* window, Camera* camera);

private:
	sf::Texture* background_texture;

	sf::Vector2f clouds_position;
	sf::Sprite* clouds_sprite;
	sf::Texture* clouds_texture;

	sf::Vector2f clouds2_position;
	sf::Sprite* clouds2_sprite;
	sf::Texture* clouds2_texture;

	sf::Vector2f hills_position;
	sf::Texture* hills_texture;
	sf::Sprite* hills_sprite;

	float findMod(float a, float b);
};