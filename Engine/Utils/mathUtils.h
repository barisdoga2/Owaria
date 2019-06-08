#pragma once

class mathUtils {
public:
	static float mathUtils::findMod(float a, float b){
		if (a < 0)
			a = -a;
		if (b < 0)
			b = -b;

		float mod = a;
		while (mod >= b)
			mod = mod - b;

		if (a < 0)
			return -mod;

		return mod;
	}

	static bool mathUtils::intersectsWith(sf::Vector2f pos, sf::Vector2u size, sf::Vector2i point) {
		return point.x >= pos.x && point.x <= pos.x + size.x && point.y >= pos.y && point.y <= pos.y + size.y;
	}

	static bool mathUtils::intersectsWith(sf::Vector2f pos, sf::Vector2f size, sf::Vector2i point) {
		return point.x >= pos.x && point.x <= pos.x + size.x && point.y >= pos.y && point.y <= pos.y + size.y;
	}

};