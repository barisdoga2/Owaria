#include <MarchingSquares.h>

MarchingSquares::MarchingSquares(Map* map) {
	this->map = map;

	const clock_t begin_time = clock();

	findLookups();
	solveAll();

	printf("Number of solutions: %d in %.0fms.\n", solutions.size(), float(clock() - begin_time));
}

void MarchingSquares::findLookups() {
	sf::Vector2i tilemapTileSize = map->getMapSize();

	for (int y = 0; y < tilemapTileSize.y - 1; y++) {
		for (int x = 0; x < tilemapTileSize.x - 1; x++) {
			if ((map->gridTiles + y * tilemapTileSize.x + x)->isSolid())
				continue;

			int bits = 0;

			int rightBit = x + 1 < tilemapTileSize.x ? (map->gridTiles + y * tilemapTileSize.x + x + 1)->isSolid() : 0;
			int topBit = y - 1 >= 0 ? (map->gridTiles + (y - 1) * tilemapTileSize.x + x)->isSolid() : 0;
			int leftBit = x - 1 >= 0 ? (map->gridTiles + y * tilemapTileSize.x + x - 1)->isSolid() : 0;
			int bottomBit = y + 1 < tilemapTileSize.y ? (map->gridTiles + (y + 1) * tilemapTileSize.x + x)->isSolid() : 0;

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

void MarchingSquares::pushSpecialTile(MarchingSolution* solution, TileData* tD, int x, int y, int xCheckDir, int yCheckDir, int axisSwap) {
	if (tD != nullptr) {
		if (axisSwap) {
			if (xCheckDir > 0) {
				for (unsigned int i = 0; i < tD->collision_vertices.size(); i++)
					solution->t_vertices.push_back(sf::Vector2i((x + xCheckDir) * 16 + tD->collision_vertices.at(i).x, (y - yCheckDir) * 16 + tD->collision_vertices.at(i).y));
			}
			else {
				for (int i = tD->collision_vertices.size() - 1; i >= 0; i--)
					solution->t_vertices.push_back(sf::Vector2i((x + xCheckDir) * 16 + tD->collision_vertices.at(i).x, (y - yCheckDir) * 16 + tD->collision_vertices.at(i).y));
			}
		}
		else {
			if (yCheckDir > 0) {
				for (int i = tD->collision_vertices.size() - 1; i >= 0; i--)
					solution->t_vertices.push_back(sf::Vector2i((x + xCheckDir) * 16 + tD->collision_vertices.at(i).x, (y - yCheckDir) * 16 + tD->collision_vertices.at(i).y));
			}
			else {
				for (unsigned int i = 0; i < tD->collision_vertices.size(); i++)
					solution->t_vertices.push_back(sf::Vector2i((x + xCheckDir) * 16 + tD->collision_vertices.at(i).x, (y - yCheckDir) * 16 + tD->collision_vertices.at(i).y));
			}
		}
	}
}

void MarchingSquares::pushSpecialTileCorner(MarchingSolution* solution, TileData* tD, int x, int y, int xCheckDir, int yCheckDir, int axisSwap) {
	if (tD != nullptr) {
		if (axisSwap) {
			if (xCheckDir > 0) {
				for (unsigned int i = 0; i < tD->collision_vertices.size(); i++)
					solution->t_vertices.push_back(sf::Vector2i((x) * 16 + tD->collision_vertices.at(i).x, (y - yCheckDir) * 16 + tD->collision_vertices.at(i).y));
			}
			else {
				for (int i = tD->collision_vertices.size() - 1; i >= 0; i--)
					solution->t_vertices.push_back(sf::Vector2i((x) * 16 + tD->collision_vertices.at(i).x, (y - yCheckDir) * 16 + tD->collision_vertices.at(i).y));
			}
		}
		else {
			if (yCheckDir > 0) {
				for (int i = tD->collision_vertices.size() - 1; i >= 0; i--)
					solution->t_vertices.push_back(sf::Vector2i((x + xCheckDir) * 16 + tD->collision_vertices.at(i).x, (y) * 16 + tD->collision_vertices.at(i).y));
			}
			else {
				for (unsigned int i = 0; i < tD->collision_vertices.size(); i++)
					solution->t_vertices.push_back(sf::Vector2i((x + xCheckDir) * 16 + tD->collision_vertices.at(i).x, (y) * 16 + tD->collision_vertices.at(i).y));
			}
		}
	}
}

MarchingSolution MarchingSquares::solve(sf::Vector2i lookup, sf::Vector2i lookupDir) {
	MarchingSolution solution;
	solution.lookup = lookup;

	sf::Vector2i tilemapTileSize = map->getMapSize();

	int x = lookup.x;
	int y = lookup.y;
	int xCheckDir = lookupDir.x;
	int yCheckDir = lookupDir.y;

	int axisSwap = xCheckDir == yCheckDir;
	while (1) {
		int front = xCheckDir == yCheckDir ? (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid() : (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid();
		int left = xCheckDir == yCheckDir ? (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid() : (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid();

		if (!front) {
			if (left) {
				if (axisSwap == 0)
					y = y - yCheckDir;
				else if (axisSwap == 1)
					x = x + xCheckDir;

				// Custom physics tile calculation here
				pushSpecialTile(&solution, (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + (x + xCheckDir))->getTileData(), x, y, xCheckDir, yCheckDir, axisSwap);

				solution.edgePoints.push_back(sf::Vector2i(x, y));
			}
			else if (!left) {
				if (axisSwap == 0) {
					yCheckDir = -yCheckDir;
					axisSwap = 1;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));

					TileData* tD = (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + (x + xCheckDir))->getTileData();
					if (tD == nullptr) {
						solution.t_vertices.push_back(sf::Vector2i((x + (xCheckDir > 0 ? 1 : 0)) * 16, (y + (yCheckDir < 0 ? 1 : 0)) * 16));
					}
					else {
						//printf("Else0");
					}
					
					front = xCheckDir == yCheckDir ? (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid() : (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid();
					left = xCheckDir == yCheckDir ? (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid() : (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid();
					if (!front) {
						x = x + xCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));

						pushSpecialTile(&solution, (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + (x + xCheckDir))->getTileData(), x, y, xCheckDir, yCheckDir, axisSwap);
					}
				}
				else if (axisSwap == 1) {
					xCheckDir = -xCheckDir;
					axisSwap = 0;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));

					TileData* tD = (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + (x + xCheckDir))->getTileData();
					if (tD == nullptr) {
						solution.t_vertices.push_back(sf::Vector2i((x + (xCheckDir > 0 ? 1 : 0)) * 16, (y + (yCheckDir < 0 ? 1 : 0)) * 16));
					}
					else {
						//printf("Else1");
					}

					front = xCheckDir == yCheckDir ? (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid() : (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid();
					left = xCheckDir == yCheckDir ? (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid() : (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid();
					if (!front) {
						y = y - yCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));

						pushSpecialTile(&solution, (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + (x + xCheckDir))->getTileData(), x, y, xCheckDir, yCheckDir, axisSwap);
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

					TileData* tD = (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->getTileData();
					if (tD == nullptr) {
						solution.t_vertices.push_back(sf::Vector2i((x + (xCheckDir < 0 ? 1 : 0)) * 16, (y + (yCheckDir < 0 ? 1 : 0)) * 16));
					}
					else {
						pushSpecialTileCorner(&solution, tD, x, y, xCheckDir, yCheckDir, axisSwap);
					}

					front = xCheckDir == yCheckDir ? (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid() : (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid();
					left = xCheckDir == yCheckDir ? (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid() : (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid();
					if (!front) {
						x = x + xCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));

						pushSpecialTile(&solution, (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + (x + xCheckDir))->getTileData(), x, y, xCheckDir, yCheckDir, axisSwap);
					}
				}
				else if (axisSwap == 1) {
					yCheckDir = -yCheckDir;
					axisSwap = 0;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));

					TileData* tD = (map->gridTiles + (y) * tilemapTileSize.x + x + xCheckDir)->getTileData();
					if (tD == nullptr) {
						solution.t_vertices.push_back(sf::Vector2i((x + (xCheckDir > 0 ? 1 : 0)) * 16, (y + (yCheckDir > 0 ? 1 : 0)) * 16));
					}
					else {
						pushSpecialTileCorner(&solution, tD, x, y, xCheckDir, yCheckDir, axisSwap);
					}

					front = xCheckDir == yCheckDir ? (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid() : (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid();
					left = xCheckDir == yCheckDir ? (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid() : (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid();
					if (!front) {
						y = y - yCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));

						pushSpecialTile(&solution, (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + (x + xCheckDir))->getTileData(), x, y, xCheckDir, yCheckDir, axisSwap);
					}
				}
			}
			else if (!left) {
				if (axisSwap == 0) {
					yCheckDir = -yCheckDir;
					axisSwap = 1;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));

					front = xCheckDir == yCheckDir ? (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid() : (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid();
					left = xCheckDir == yCheckDir ? (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid() : (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid();
					if (!front) {
						x = x + xCheckDir;
						solution.edgePoints.push_back(sf::Vector2i(x, y));

						pushSpecialTile(&solution, (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + (x + xCheckDir))->getTileData(), x, y, xCheckDir, yCheckDir, axisSwap);
					}
				}
				else if (axisSwap == 1) {
					xCheckDir = -xCheckDir;
					axisSwap = 0;
					solution.corners.push_back(sf::Vector2i(x, y));
					solution.edgePoints.push_back(sf::Vector2i(x, y));

					front = xCheckDir == yCheckDir ? (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid() : (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid();
					left = xCheckDir == yCheckDir ? (map->gridTiles + (y - yCheckDir) * tilemapTileSize.x + x)->isSolid() : (map->gridTiles + y * tilemapTileSize.x + x + xCheckDir)->isSolid();
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