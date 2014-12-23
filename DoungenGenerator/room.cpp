#include "doungen.hpp"
#include <iostream>

namespace doungen {
	bool isConnector(Room* room, Map& map, int i, int j);

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
		
		return connectors;
	}

	bool isInside(Room* room, Room* bounds) {
		int x = bounds->x;
		int y = bounds->y;
		int width = bounds->width;
		int height = bounds->height;
		return room->x >= x && room->y >= y && room->x < x + width && room->y < y + height
				|| room->x + room->width >= x && room->y >= y && room->x + room->width < x + width && room->y < y + height
				|| room->x + room->width >= x && room->y + room->height >= y && room->x + room->width < x + width && room->y + room->height < y + height
				|| room->x >= x && room->y + room->height >= y && room->x < x + width && room->y + room->height < y + height;
	}

	bool Room::intersects(std::shared_ptr<Region> region) {
		std::shared_ptr<Room> room = std::dynamic_pointer_cast<Room>(region);
		if (room) {
			return isInside(room.get(), this) || isInside(this, room.get());
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