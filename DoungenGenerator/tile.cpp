#include "doungen.hpp"

namespace doungen {
	std::array<Tile, 4> Tile::getNeighbors() {
		std::array<Tile, 4> neighbors;
		neighbors[0] = Tile(x - 1, y);
		neighbors[1] = Tile(x + 1, y);
		neighbors[2] = Tile(x, y - 1);
		neighbors[3]= Tile(x, y + 1);
		return neighbors;
	}

	std::array<Tile, 4> Tile::getExtendedNeighbors() {std::array<Tile, 4> neighbors;
		neighbors[0] = Tile(x - 2, y);
		neighbors[1] = Tile(x + 2, y);
		neighbors[2] = Tile(x, y - 2);
		neighbors[3]= Tile(x, y + 2);
		return neighbors;
	}

	bool Tile::operator ==(const Tile& tile) const {
		return tile.x == x && tile.y == y;
	}
}