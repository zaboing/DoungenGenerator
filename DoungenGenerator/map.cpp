#include "doungen.hpp"
#include <random>
#include <iostream>
#include <time.h>

namespace doungen {
	

	std::shared_ptr<Region> Map::getRegion(int x, int y) {
		if (!isInside(x, y)) {
			return std::shared_ptr<Region>(nullptr);
		}

		auto region = tileMap[x][y];

		return region;
	}

	void Map::generateRooms(int attempts) {
		std::uniform_int_distribution<int> xDistribution(0, this->width - 1);
		std::uniform_int_distribution<int> yDistribution(0, this->height - 1);
		std::uniform_int_distribution<int> widthDistribution(8, 15);
		std::uniform_int_distribution<int> heightDistribution(8, 29);
		for (int i = 0; i < attempts; i++) {
			bool intersects = false;
			std::shared_ptr<Room> room = std::make_shared<Room>(xDistribution(generator), yDistribution(generator), widthDistribution(generator), heightDistribution(generator));
			if (room->x % 2 == 0) {
				room->x += 1;
			}
			if (room->y % 2 == 0) {
				room->y += 1;
			}
			if (room->width % 2 == 0) {
				room->width += 1;
			}
			if (room->height % 2 == 0) {
				room->height += 1;
			}
			room->applyTiles();
			for (auto region : regions) {
				if (room->intersects(region)) {
					intersects = true;
					break;
				}
			}
			/*if (getRegion(room->x, room->y + room->height) || getRegion(room->x + room->width, room->y) ||
				getRegion(room->x + room->width, room->y + room->height) || getRegion(room->x, room->y)) {
					continue;
			}*/
			if (!intersects) {
				//room->shrink(1, 1);
				regions.push_back(room);
			}
		}
	
		update();
	}

	void Map::generateCorridor(int startX, int startY) {
		auto corridor = std::make_shared<Corridor>(startX, startY);
		regions.push_back(corridor);
		corridor->generate(*this);
		update();
	}

	void Map::shrinkCorridors(float accuracy) {
		for (auto region : regions) {
			auto corridor = std::dynamic_pointer_cast<Corridor>(region);
			if (corridor) {
				corridor->shrink(accuracy, *this);
				update();
			}
		}
	}

	void Map::update() {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				tileMap[i][j] = std::shared_ptr<Region>(nullptr);
			}
		}
		for (auto region : regions) {
			for (auto tile : region->tiles) {
				if (isInside(tile)) {
					tileMap[tile.x][tile.y] = region;
				}
			}
		}
	}
	
	bool Map::isInside(int x, int y) const {
		return x > 0 && y > 0 && x < width && y < height;
	}
	bool Map::isInside(Tile tile) const {
		return isInside(tile.x, tile.y);
	}
}