#include <Player.h>


Player::Player() {
	std::string playerPath = "../../Resources/Player/";

	spritesheet = new sf::Image();
	spritesheet->loadFromFile(playerPath + "Player.png");

	texture = new sf::Texture();
	texture->loadFromImage(*spritesheet, sf::IntRect(0,0,64,64));
}

Player::~Player() {
	delete spritesheet;
	delete texture;
}

void Player::Render(sf::RenderWindow* window) {
	sf::RectangleShape r;
	r.setSize(sf::Vector2f(64,64));
	r.setPosition(position);
	r.setTexture(texture);
	window->draw(r);
}

void Player::Update(int updateElapsed) {
	position.x += velocity.x;
	position.y += velocity.y;
}

void Player::HandleInputs() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocity.x = -1;
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocity.x = 1;
	}
	else
		velocity.x = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		velocity.y = -1;
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		velocity.y = 1;
	}
	else
		velocity.y = 0.0f;
}