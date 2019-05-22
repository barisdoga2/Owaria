#include <Effect.h>


Effect::Effect(string effectName) {
	this->effectName = effectName;

	// Load effect from cfg to animation here
	string pathToEffect ="../../Resources/Effects/" + effectName;

	img = new sf::Image();
	img->loadFromFile(pathToEffect + "/" + effectName + ".png");
	
	int effectLength, yStart, width, height, frameDelay, isContinious;
	std::ifstream infile(pathToEffect + "/" + effectName + ".cfg");
	std::istringstream stream("");
	ioUtils::getNextLine(stream, infile);
	stream >> effectLength >> yStart >> width >> height >> frameDelay >> isContinious;

	effectAnim = new Animation(img, effectName, yStart, width, height, effectLength, frameDelay, isContinious == 1);
}

Effect::~Effect() {
	delete img;
	delete effectAnim;
}

void Effect::Start() {
	effectAnim->Show();
	effectAnim->Play();
}

void Effect::Render(sf::RenderWindow* window, sf::Vector2f offset) {
	effectAnim->Render(window, offset);
}

void Effect::Update(int updateElapsed) {
	effectAnim->Update(updateElapsed);
}


Effect* Effect::getClone() {
	Effect* retVal = nullptr;

	return retVal;
}