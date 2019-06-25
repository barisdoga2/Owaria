#include <Animation.h>



Animation::Animation(std::string name, sf::Vector2i startPosition, sf::Vector2i size, int length, int frameDelay, bool isContinuous) {
	this->name = name;
	this->size = size;
	this->startPosition = startPosition;
	this->frameDelay = frameDelay;
	this->isContinuous = isContinuous;
	this->length = length;
	this->currentFrame = 0;
	for (int i = 0; i < length; i++) 
		frameCoords.push_back(sf::Vector2i(startPosition.x + i * size.x, startPosition.y));
}

Animation::~Animation() {

}

void Animation::Hide() {
	isHidden = true;
}

void Animation::Show() {
	isHidden = false;
}

void Animation::Play() {
	currentFrame = 0;
	timePassedMs = 0;
	isPlaying = 1;
}

void Animation::Stop() {
	isPlaying = 0;
	currentFrame = length - 1;
}

bool Animation::isFinished() {
	return isPlaying == 0;
}

void Animation::Update(int updateElapsed) {
	if (isPlaying) {
		timePassedMs += updateElapsed;
		if (timePassedMs >= frameDelay * (currentFrame + 1))
			currentFrame++;
		if (currentFrame > length - 1)
			if (isContinuous)
				Play();
			else
				Stop();
	}
}

void Animation::Render(sf::RenderWindow* window, sf::Texture* texture, sf::Vector2f coords, int yMirror) {
	if (isHidden)
		return;

	renderer.setTexture(*texture);
	renderer.setPosition(sf::Vector2f(coords.x + (yMirror ? size.x : 0), coords.y));
	renderer.setTextureRect(sf::IntRect(frameCoords.at(currentFrame), size));

	if (!yMirror)
		renderer.setScale(1, 1);
	else
		renderer.setScale(-1, 1);

	window->draw(renderer);
}

string Animation::GetName() {
	return this->name;
}

int Animation::GetCurrentFrame() {
	return this->currentFrame;
}

vector<sf::Vector2i> Animation::GetFrameCoords() {
	return this->frameCoords;
}

sf::Vector2i Animation::getStartPosition() {
	return this->startPosition;
}

sf::Vector2i Animation::getSize() {
	return this->size;
}