#include <Background.h>


Background::Background(Camera* camera) {
	this->camera = camera;

	background_texture = new sf::Texture();
	background_texture->loadFromFile("../../Resources/Backgrounds/background.png");

	// Create Clouds
	clouds_position.x = 0;
	clouds_sprite = new sf::Sprite();
	clouds_texture = new sf::Texture();
	clouds_texture->loadFromFile("../../Resources/Backgrounds/distant_clouds.png");
	clouds_sprite->setTexture(*clouds_texture);
	clouds_sprite->setScale(SCREEN_WIDTH / (float)clouds_texture->getSize().x, SCREEN_HEIGHT / (float)clouds_texture->getSize().y);

	clouds2_position.x = 0;
	clouds2_sprite = new sf::Sprite();
	clouds2_texture = new sf::Texture();
	clouds2_texture->loadFromFile("../../Resources/Backgrounds/far_distant_clouds.png");
	clouds2_sprite->setTexture(*clouds2_texture);
	clouds2_sprite->setScale(SCREEN_WIDTH / (float)clouds2_texture->getSize().x, SCREEN_HEIGHT / (float)clouds2_texture->getSize().y);

	hills_sprite = new sf::Sprite();
	hills_texture = new sf::Texture();
	hills_texture->loadFromFile("../../Resources/Backgrounds/hills.png");
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
	clouds_position.x = findMod(clouds_position.x, clouds_sprite->getGlobalBounds().width);

	move = (updateElapsed / 1000.0f) * (CLOUDS_SPEED / SLOW_CLOUD_DIV);
	if (camera->isOffsetsLocked().x == 0)
		move -= camera->GetTarget()->body->GetLinearVelocity().x / PLAYER_CLOUD_MULT;
	clouds2_position.x += move;
	clouds2_position.x = findMod(clouds2_position.x, clouds2_sprite->getGlobalBounds().width);

	// Update Hills
	hills_position.x = (float)(-(int)hills_position.x % (int)hills_sprite->getGlobalBounds().width);
}

void Background::Render(sf::RenderWindow* window) {
	sf::Sprite sprite;
	sprite.setTexture(*background_texture);
	sprite.setScale(SCREEN_WIDTH / (float)background_texture->getSize().x, SCREEN_HEIGHT / (float)background_texture->getSize().y);
	window->draw(sprite);

	// Render Clouds
	sf::Vector2f renderPos((float)-((int)(camera->getPosition().x + clouds_position.x) % (int)clouds_sprite->getGlobalBounds().width), clouds_position.y);
	clouds_sprite->setPosition(sf::Vector2f(renderPos.x - clouds_sprite->getGlobalBounds().width, clouds_position.y));
	window->draw(*clouds_sprite);
	clouds_sprite->setPosition(renderPos);
	window->draw(*clouds_sprite);
	clouds_sprite->setPosition(sf::Vector2f(renderPos.x + clouds_sprite->getGlobalBounds().width, clouds_position.y));
	window->draw(*clouds_sprite);

	renderPos = sf::Vector2f((float)-((int)(camera->getPosition().x + clouds2_position.x) % (int)clouds2_sprite->getGlobalBounds().width), clouds2_position.y);
	clouds2_sprite->setPosition(sf::Vector2f(renderPos.x - clouds2_sprite->getGlobalBounds().width, clouds2_position.y));
	window->draw(*clouds2_sprite);
	clouds2_sprite->setPosition(renderPos);
	window->draw(*clouds2_sprite);
	clouds2_sprite->setPosition(sf::Vector2f(renderPos.x + clouds2_sprite->getGlobalBounds().width, clouds2_position.y));
	window->draw(*clouds2_sprite);

	// Render Hills
	renderPos = sf::Vector2f((float)-((int)(camera->getPosition().x + hills_position.x) % (int)hills_sprite->getGlobalBounds().width), hills_position.y);
	hills_sprite->setPosition(sf::Vector2f(renderPos.x - hills_sprite->getGlobalBounds().width, clouds_position.y));
	window->draw(*hills_sprite);
	hills_sprite->setPosition(renderPos);
	window->draw(*hills_sprite);
	hills_sprite->setPosition(sf::Vector2f(renderPos.x + hills_sprite->getGlobalBounds().width, clouds_position.y));
	window->draw(*hills_sprite);

}

float Background::findMod(float a, float b)
{
	// Handling negative values 
	if (a < 0)
		a = -a;
	if (b < 0)
		b = -b;

	// Finding mod by repeated subtraction 
	float mod = a;
	while (mod >= b)
		mod = mod - b;

	// Sign of result typically depends 
	// on sign of a. 
	if (a < 0)
		return -mod;

	return mod;
}