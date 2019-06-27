#include <Animation.h>



Animation::Animation(AnimationAsset* animationAsset, bool isContinuous) {
	this->animationAsset = animationAsset;

	this->isContinuous = isContinuous;
	this->currentFrame = 0;

	this->isPlaying = false;
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
	currentFrame = animationAsset->getFrameCount() - 1;
}

bool Animation::isFinished() {
	return isPlaying == 0;
}

void Animation::Update(int updateElapsed) {
	if (isPlaying) {
		timePassedMs += updateElapsed;
		if (timePassedMs >= animationAsset->getFrameDelay() * (currentFrame + 1))
			currentFrame++;
		if (currentFrame > animationAsset->getFrameCount() - 1)
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
	renderer.setPosition(sf::Vector2f(coords.x + (yMirror ? animationAsset->getSize().x : 0), coords.y));
	renderer.setTextureRect(sf::IntRect(animationAsset->getFrameCoords().at(currentFrame), animationAsset->getSize()));

	if (!yMirror)
		renderer.setScale(1, 1);
	else
		renderer.setScale(-1, 1);

	window->draw(renderer);
}


int Animation::GetCurrentFrame() {
	return this->currentFrame;
}

AnimationAsset* Animation::getAnimationAsset() {
	return this->animationAsset;
}

string Animation::getAnimationAssetName() {
	return this->animationAsset->getName();
}