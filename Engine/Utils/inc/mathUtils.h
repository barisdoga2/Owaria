#pragma once

class mathUtils {
public:

	// Mod of two floats
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

};