#include <Background.h>


Background::Background(Camera* camera) {
	this->camera = camera;

	background_texture = new sf::Texture();
	background_texture->loadFromFile("../../Resources/Backgrounds/_11_background.png");

	// Create Clouds
	clouds_position.x = 0;
	clouds_sprite = new sf::Sprite();
	clouds_texture = new sf::Texture();
	clouds_texture->loadFromFile("../../Resources/Backgrounds/_08_clouds.png");
	clouds_sprite->setTexture(*clouds_texture);
	clouds_sprite->setScale(SCREEN_WIDTH / (float)clouds_texture->getSize().x, SCREEN_HEIGHT / (float)clouds_texture->getSize().y);


	hills_sprite = new sf::Sprite();
	hills_texture = new sf::Texture();
	hills_texture->loadFromFile("../../Resources/Backgrounds/_06_hill2.png");
	hills_sprite->setTexture(*hills_texture);
	hills_sprite->setScale(SCREEN_WIDTH / (float)hills_texture->getSize().x, SCREEN_HEIGHT / (float)hills_texture->getSize().y);
}

Background::~Background() {
	delete background_texture;
	delete clouds_texture;
	delete clouds_sprite;
}

void Background::Update(int updateElapsed) {
	// Update Clouds
	float move = (updateElapsed / 1000.0f) * CLOUDS_SPEED;
	if (camera->isOffsetsLocked().x == 0)
		move -= camera->GetTarget()->body->GetLinearVelocity().x / PLAYER_CLOUD_MULT;
	
	clouds_position.x += move;
	clouds_position.x = (float)((int)clouds_position.x % (int)clouds_sprite->getGlobalBounds().width);

	// Update Hills
	hills_position.x = (float)(-(int)hills_position.x % (int)hills_sprite->getGlobalBounds().width);
}

void Background::Render(sf::RenderWindow* window) {

	sf::Sprite sprite;
	sprite.setTexture(*background_texture);
	sprite.setScale(SCREEN_WIDTH / (float)background_texture->getSize().x, SCREEN_HEIGHT / (float)background_texture->getSize().y);
	window->draw(sprite);

	// Render Clouds
	sf::Vector2f renderPos((float)-((int)(camera->GetMap()->m_offset.x + clouds_position.x) % (int)clouds_sprite->getGlobalBounds().width), clouds_position.y);
	clouds_sprite->setPosition(sf::Vector2f(renderPos.x - clouds_sprite->getGlobalBounds().width, clouds_position.y));
	window->draw(*clouds_sprite);
	clouds_sprite->setPosition(renderPos);
	window->draw(*clouds_sprite);
	clouds_sprite->setPosition(sf::Vector2f(renderPos.x + clouds_sprite->getGlobalBounds().width, clouds_position.y));
	window->draw(*clouds_sprite);

	// Render Hills
	renderPos = sf::Vector2f((float)-((int)(camera->GetMap()->m_offset.x + hills_position.x) % (int)hills_sprite->getGlobalBounds().width), hills_position.y);
	hills_sprite->setPosition(sf::Vector2f(renderPos.x - hills_sprite->getGlobalBounds().width, clouds_position.y));
	window->draw(*hills_sprite);
	hills_sprite->setPosition(renderPos);
	window->draw(*hills_sprite);
	hills_sprite->setPosition(sf::Vector2f(renderPos.x + hills_sprite->getGlobalBounds().width, clouds_position.y));
	window->draw(*hills_sprite);

}