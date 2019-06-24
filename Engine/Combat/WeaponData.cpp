#include <WeaponData.h>


WeaponData::WeaponData(std::string name, Animation animation, sf::Texture spriteSheet) {
	this->name = name;

	// Extract Bounding Chain Fixture Points
	const sf::Image im = spriteSheet.copyToImage();
	const int imWidth = im.getSize().x;
	const sf::Uint8* pixels = im.getPixelsPtr();

	for (sf::IntRect frame : animation.GetFrames()) {
		vector<sf::Vector3i> points;

		for (int y = frame.top; y < frame.top + frame.height; y++) {
			for (int x = frame.left; x < frame.left + frame.width; x++) {
				const sf::Uint8* offset = pixels + (x + y * imWidth) * 4;
				int r = (int)*(offset + 0);
				int g = (int)*(offset + 1);
				int b = (int)*(offset + 2);
				int a = (int)*(offset + 3);
				if (r == 127 && g == 1 && b == 127)
					points.push_back(sf::Vector3i(x % 64, y % 64, a));
			}
		}

		sort(points.begin(), points.end(), [](const sf::Vector3i& lhs, const sf::Vector3i& rhs) {
			return lhs.z < rhs.z;
		});

		frame_points.push_back(points);
	}

}

WeaponData::~WeaponData() {

}