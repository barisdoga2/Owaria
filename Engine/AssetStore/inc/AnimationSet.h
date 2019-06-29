#pragma once

#include <iostream>
#include <vector>
#include <tinyxml2.h>
#include <AnimationAsset.h>

using namespace std;
using namespace tinyxml2;

class AnimationSet {

public:
	AnimationSet(XMLElement* animationsetElement);
	~AnimationSet();

	string getName();
	AnimationAsset* getAnimationAsset(string name);
	vector<AnimationAsset*> getAllAnimationAssets();

private:
	string name;
	vector<AnimationAsset*> animationAssets;

};