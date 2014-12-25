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
		std::uniform_int_distribution<int> widthDistribution(3, 15);
		std::uniform_int_distribution<int> heightDistribution(3, 15);
		char id = 'A';
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
			if (!isInside(room->x, room->y) || !isInside(room->x + room->width, room->y + room->height)
				|| !isInside(room->x + room->width, room->y) ||!isInside(room->x, room->y + room->height)) {
					continue;
			}
			room->applyTiles();
			for (auto region : regions) {
				if (room->intersects(region)) {
					intersects = true;
					break;
				}
			}
			if (!intersects) {
				room->id = id++;
				std::cout << room->id << ":" << room->x << "." << room->y << " " << room->width << "x" << room->height << std::endl;
				//room->shrink(1, 1);
				regions.push_back(room);
				set(room->tiles, room);
			}
		}
	}

	void Map::generateCorridor(int startX, int startY) {
		auto corridor = std::make_shared<Corridor>(startX, startY);
		regions.push_back(corridor);
		set(corridor->tiles, corridor);
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

	void Map::unset(Tile& tile) {
		if (isInside(tile)) {
			tileMap[tile.x][tile.y] = std::shared_ptr<Region>(nullptr);
		}
	}

	void Map::unset(std::vector<Tile>& tiles) {
		for (auto tile : tiles) {
			unset(tile);
		}
	}

	std::shared_ptr<Region> Map::getRegion(Region* region) {
		if (region != nullptr) {
			for (auto reg : regions) {
				if (reg.get() == region) {
					return reg;
				}
			}
		}
		return std::shared_ptr<Region>(nullptr);
	}
	
	void Map::set(Tile& tile, Region* region) {
		set(tile, getRegion(region));
	}

	void Map::set(Tile& tile, std::shared_ptr<Region> region) {
		if (isInside(tile)) {
			tileMap[tile.x][tile.y] = region;
		}
	}

	void Map::set(std::vector<Tile>& tiles, Region* region) {
		set(tiles, getRegion(region));
	}

	void Map::set(std::vector<Tile>& tiles, std::shared_ptr<Region> region) {
		for (auto tile : tiles) {
			set(tile, region);
		}
	}
	
	bool Map::isInside(int x, int y) const {
		return x > 0 && y > 0 && x < width && y < height;
	}
	bool Map::isInside(Tile tile) const {
		return isInside(tile.x, tile.y);
	}
}