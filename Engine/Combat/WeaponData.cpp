#include <WeaponData.h>


WeaponData::WeaponData(std::string name, AnimationAsset* animationAsset, sf::Texture spriteSheet) {
	this->name = name;

	// Extract Bounding Chain Fixture Points
	const sf::Image im = spriteSheet.copyToImage();
	const int imWidth = im.getSize().x;
	const sf::Uint8* pixels = im.getPixelsPtr();

	for (sf::Vector2i frame : animationAsset->getFrameCoords()) {
		vector<sf::Vector3i> points;
		for (int y = frame.y; y < frame.y + animationAsset->getSize().y; y++) {
			for (int x = frame.x; x < frame.x + animationAsset->getSize().x; x++) {
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

		vector<sf::Vector2i> points2D;
		vector<sf::Vector2i> points2D_yMirror;
		for (sf::Vector3i v : points) {
			points2D.push_back(sf::Vector2i(v.x, v.y));
			points2D_yMirror.push_back(sf::Vector2i(animationAsset->getSize().x - v.x, v.y));
		}

		frame_points.push_back(points2D);
		frame_points_yMirror.push_back(points2D_yMirror);
	}

}

WeaponData::~WeaponData() {

}

vector<sf::Vector2i> WeaponData::GetFramePoints(int frameNum, bool yMirror) {
	if (!yMirror)
		return frame_points.at(frameNum);
	else
		return frame_points_yMirror.at(frameNum);
}