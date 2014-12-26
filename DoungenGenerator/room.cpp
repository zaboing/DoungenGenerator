#include "doungen.hpp"

namespace doungen {
	bool isConnector(Room* room, Map& map, int i, int j);

	int min(int a, int b) {
		if (a < b) return a;
		return b;
	}

	Room::Room(int xPos, int yPos, int w, int h) : x(xPos), y(yPos), width(w), height(h) {
		applyTiles();
    }

	std::vector<Tile> Room::connectors(Map& map) {
		std::vector<Tile> connectors;

		for (int i = 0; i < width; i++) {
			Tile tile(x + i, y - 1);
			if (isConnector(this, map, tile.x, tile.y)) {
				connectors.push_back(tile);
			}
			tile = Tile(x + i, y + width);
			if (isConnector(this, map, tile.x, tile.y)) {
				connectors.push_back(tile);
			}
		}
		
		for (int i = 0; i < height; i++) {
			Tile tile(x - 1, y + i);
			if (isConnector(this, map, tile.x, tile.y)) {
				connectors.push_back(tile);
			}
			tile = Tile(x + width, y + i);
			if (isConnector(this, map, tile.x, tile.y)) {
				connectors.push_back(tile);
			}
		}

		if (connectors.size() > 0) {
			std::uniform_int_distribution<int> amountDistribution(1, min(4, connectors.size()));
			int amount = amountDistribution(map.generator);
			for (int i = amount; i > 0; --i) {
				std::uniform_int_distribution<int> indexDistribution(0, connectors.size() - 1);
				int index = indexDistribution(map.generator);
				tiles.push_back(connectors[index]);
				map.set(connectors[index], this);
				connectors.erase(connectors.begin() + index);
			}
		}

		return connectors;
	}

	bool Room::intersects(std::shared_ptr<Region> region) {
		std::shared_ptr<Room> room = std::dynamic_pointer_cast<Room>(region);
		if (room) {
			return !(room->x > this->x + this->width || 
           room->x + room->width < this->x || 
           room->y > this->y + this->height ||
           room->y + room->height < this->y);
		} else {
			return false;
		}
	}

	void Room::shrink(int amountX, int amountY) {
		x += amountX;
		y += amountY;
		width -= amountX;
		height -= amountY;
		applyTiles();
	}

	void Room::applyTiles() {
		tiles.clear();
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				Tile tile(i + x, j + y);
				tiles.push_back(tile);
			}
		}
	}

	bool isConnector(Room* room, Map& map, int i, int j) {
		if (!map.isInside(i, j)) {
			return false;
		}
		Tile tile(i, j);
		std::shared_ptr<Region> region = map.getRegion(tile);
		if (region) {
			return false;
		}
		std::array<Tile, 4> neighbors = tile.getNeighbors();
		for (uint32_t i = 0; i < neighbors.size(); i++) {
			if (map.isInside(neighbors[i])) {
				std::shared_ptr<Region> neighborRegion = map.getRegion(neighbors[i]);
				if (neighborRegion && !(neighborRegion.get() == room)) {
					return true;
				}
			}
		}
		return false;
	}
}