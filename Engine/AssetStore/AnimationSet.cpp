#include <AnimationSet.h>


AnimationSet::AnimationSet(XMLElement* animationsetElement) {
	this->name = animationsetElement->Attribute("name");

	XMLElement* object = animationsetElement->FirstChildElement("AnimationAsset");
	while (object != NULL) {
		XMLElement* startPosition = object->FirstChildElement("startPosition");
		XMLElement* size = object->FirstChildElement("size");
		animationAssets.push_back(new AnimationAsset(object->Attribute("name"), sf::Vector2i(startPosition->IntAttribute("x"), startPosition->IntAttribute("y")), sf::Vector2i(size->IntAttribute("width"), size->IntAttribute("height")), object->IntAttribute("frameCount"), object->IntAttribute("frameDelay")));
		object = object->NextSiblingElement();
	}
}

AnimationSet::~AnimationSet() {
	for (AnimationAsset* a : animationAssets)
		delete a;
}

string AnimationSet::getName() {
	return this->name;
}

AnimationAsset* AnimationSet::getAnimationAsset(string name) {
	for (AnimationAsset* animationAsset : animationAssets)
		if (animationAsset->getName().compare(name) == 0)
			return animationAsset;

	return nullptr;
}