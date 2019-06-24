#include <Animation.h>



Animation::Animation(std::string name, int xStart, int yStart, int width, int height, int length, int frameDelay, bool isContinuous) {
	this->name = name;
	this->frameDelay = frameDelay;
	this->width = width;
	this->height = height;
	this->isContinuous = isContinuous;
	this->length = length;
	this->currentFrame = 0;
	for (int i = 0; i < length; i++) 
		frames.push_back(sf::IntRect(xStart + i * width, yStart, width, height));
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
	renderer.setPosition(sf::Vector2f(coords.x + (yMirror ? width : 0), coords.y));
	renderer.setTextureRect(frames.at(currentFrame));

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

vector<sf::IntRect> Animation::GetFrames() {
	return this->frames;
}