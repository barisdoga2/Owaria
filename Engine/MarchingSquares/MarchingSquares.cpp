#include <MarchingSquares.h>

MarchingSquares::MarchingSquares(Map* map) {
	this->map = map;

	const clock_t begin_time = clock();

	findLookups();
	solveAll();

	printf("Number of solutions: %d in %.0fms.\n", solutions.size(), float(clock() - begin_time));
}

void MarchingSquares::findLookups() {
	int width = map->getMapWidth();
	int height = map->getMapHeight();
	int* bitmap = map->gridTileIDs;

	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {
			if (*(bitmap + y * width + x) == 1)
				continue;

			int bits = 0;

			int rightBit = x + 1 < width ? *(bitmap + y * width + x + 1) : 0;
			int topBit = y - 1 >= 0 ? *(bitmap + (y - 1) * width + x) : 0;
			int leftBit = x - 1 >= 0 ? *(bitmap + y * width + x - 1) : 0;
			int bottomBit = y + 1 < height ? *(bitmap + (y + 1) * width + x) : 0;

			bits += rightBit;
			bits += topBit;
			bits += leftBit;
			bits += bottomBit;

			if (bits == 1) {
				if (rightBit)
					lookupDirs.push_back(sf::Vector2i(1, -1));
				else if (topBit)
					lookupDirs.push_back(sf::Vector2i(1, 1));
				else if (leftBit)
					lookupDirs.push_back(sf::Vector2i(-1, 1));
				else if (bottomBit)
					lookupDirs.push_back(sf::Vector2i(-1, -1));
			}

			if (bits == 1)
				lookups.push_back(sf::Vector2i(x, y));
		}
	}
}

void MarchingSquares::solveAll() {
	int lookup = 0;
	for (sf::Vector2i p : lookups) {
		bool found = 0;
		for (MarchingSolution s : solutions) {
			for (sf::Vector2i edge : s.edgePoints) {
				if (edge.x == p.x && edge.y == p.y) {
					found = 1;
					break;
				}
			}
			if (found == 1)
				break;
		}
		if (!found)
			solutions.push_back(solve(p, lookupDirs.at(lookup)));

		lookup++;
	}
}

MarchingSolution MarchingSquares::solve(sf::Vector2i lookup, sf::Vector2i lookupDir) {
	MarchingSolution solution;
	solution.lookup = lookup;

	int width = map->getMapWidth();
	int height = map->getMapHeight();
	int* bitmap = map->gridTileIDs;
	int x = lookup.x;
	int y = lookup.y;
	int xCheckDir = lookupDir.x;
	int yCheckDir = lookupDir.y;

	int axisSwap = xCheckDir == yCheckDir;
	while (1) {
		int front = xCheckDir == yCheckDir ? *(bitmap + y * width + x + xCheckDir) : *(bitmap + (y - yCheckDir) * width + x);
		int left = xCheckDir == yCheckDir ? *(bitmap + (y - yCheckDir) * width + x) : *(bitmap + y * width + x + xCheckDir);

		if (!front) {
			if (left) {
				if (axisSwap == 0)
					y = y - yCheckDir;
				else if (axisSwap == 1)
					x = x + xCheckDir;

				solution.edgePoints.push_back(sf::Vector2i(x, y));
			}
			else if (!left) {
				if (axisSwap == 0) {
					yCheckDir = -yCheckDir;
					axisSwap = 1;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));
					solution.t_vertices.push_back(sf::Vector2i((x + (xCheckDir > 0 ? 1 : 0)) * 16, (y + (yCheckDir < 0 ? 1 : 0)) * 16));

					front = xCheckDir == yCheckDir ? *(bitmap + y * width + x + xCheckDir) : *(bitmap + (y - yCheckDir) * width + x);
					left = xCheckDir == yCheckDir ? *(bitmap + (y - yCheckDir) * width + x) : *(bitmap + y * width + x + xCheckDir);
					if (!front) {
						x = x + xCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));
					}
				}
				else if (axisSwap == 1) {
					xCheckDir = -xCheckDir;
					axisSwap = 0;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));
					solution.t_vertices.push_back(sf::Vector2i((x + (xCheckDir > 0 ? 1 : 0)) * 16, (y + (yCheckDir < 0 ? 1 : 0)) * 16));

					front = xCheckDir == yCheckDir ? *(bitmap + y * width + x + xCheckDir) : *(bitmap + (y - yCheckDir) * width + x);
					left = xCheckDir == yCheckDir ? *(bitmap + (y - yCheckDir) * width + x) : *(bitmap + y * width + x + xCheckDir);
					if (!front) {
						y = y - yCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));
					}
				}
			}
		}
		else if (front) {
			if (left) {
				if (axisSwap == 0) {
					xCheckDir = -xCheckDir;
					axisSwap = 1;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));
					solution.t_vertices.push_back(sf::Vector2i((x + (xCheckDir < 0 ? 1 : 0)) * 16, (y + (yCheckDir < 0 ? 1 : 0)) * 16));

					front = xCheckDir == yCheckDir ? *(bitmap + y * width + x + xCheckDir) : *(bitmap + (y - yCheckDir) * width + x);
					left = xCheckDir == yCheckDir ? *(bitmap + (y - yCheckDir) * width + x) : *(bitmap + y * width + x + xCheckDir);
					if (!front) {
						x = x + xCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));
					}
				}
				else if (axisSwap == 1) {
					yCheckDir = -yCheckDir;
					axisSwap = 0;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));
					solution.t_vertices.push_back(sf::Vector2i((x + (xCheckDir > 0 ? 1 : 0)) * 16, (y + (yCheckDir > 0 ? 1 : 0)) * 16));

					front = xCheckDir == yCheckDir ? *(bitmap + y * width + x + xCheckDir) : *(bitmap + (y - yCheckDir) * width + x);
					left = xCheckDir == yCheckDir ? *(bitmap + (y - yCheckDir) * width + x) : *(bitmap + y * width + x + xCheckDir);
					if (!front) {
						y = y - yCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));
					}
				}
			}
			else if (!left) {
				if (axisSwap == 0) {
					yCheckDir = -yCheckDir;
					axisSwap = 1;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));

					front = xCheckDir == yCheckDir ? *(bitmap + y * width + x + xCheckDir) : *(bitmap + (y - yCheckDir) * width + x);
					left = xCheckDir == yCheckDir ? *(bitmap + (y - yCheckDir) * width + x) : *(bitmap + y * width + x + xCheckDir);
					if (!front) {
						x = x + xCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));
					}
				}
				else if (axisSwap == 1) {
					xCheckDir = -xCheckDir;
					axisSwap = 0;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));

					front = xCheckDir == yCheckDir ? *(bitmap + y * width + x + xCheckDir) : *(bitmap + (y - yCheckDir) * width + x);
					left = xCheckDir == yCheckDir ? *(bitmap + (y - yCheckDir) * width + x) : *(bitmap + y * width + x + xCheckDir);
					if (!front) {
						y = y - yCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));
					}
				}
			}
		}

		if (x == lookup.x && y == lookup.y)
			break;
	}

	return solution;
}