#include "doungen.hpp"

#include <iostream>
#include <time.h>

namespace doungen {

	Corridor::Corridor(Tile start) {
		tiles.push_back(start);
	}

	Corridor::Corridor(int startX, int startY) {
		tiles.push_back(Tile(startX, startY));
	}

	void Corridor::generate(Map& map) {
		std::vector<Tile> candidates;
		for (auto tile : tiles) {
			if (tile.x % 2 == 0 || tile.y % 2 == 0) { // EVEN tile
				continue;
			}
			std::array<Tile, 4> xNeighbors = tile.getExtendedNeighbors();
			int empty = 0;
			for (uint32_t i = 0; i < xNeighbors.size(); i++) {
				if (!map.getRegion(xNeighbors[i])) {
					empty++;
				}
			}
			if (empty > 0) {
				candidates.push_back(tile);
			}
		}
		time_t timer;
		struct tm y2k;
		double seconds;

		y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
		y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;
		
		time(&timer);  /* get current time; same as: timer = time(NULL)  */

		seconds = difftime(timer,mktime(&y2k));

		generate(map, candidates);

		time(&timer);  /* get current time; same as: timer = time(NULL)  */

		std::cout << "Generated corridors in " << (difftime(timer,mktime(&y2k)) - seconds) << " seconds." << std::endl;
	}

	void Corridor::generate(Map& map, std::vector<Tile>& candidates) {
election:
		if (candidates.empty()) {
			return;
		}
		std::uniform_int_distribution<int> candidateDistr(0, candidates.size() - 1);
		int index = candidateDistr(map.generator);
			
		std::array<Tile, 4> xNeighbors = candidates[index].getExtendedNeighbors();
		int empty = 0;
		for (uint32_t i = 0; i < xNeighbors.size(); i++) {
			if (!map.getRegion(xNeighbors[i]) && map.isInside(xNeighbors[i])) {
				empty++;
			}
		}
		if (empty == 0) {
			candidates.erase(candidates.begin() + index);
			goto election;
		}

		carveNext(candidates[index], map);

		Tile added = tiles[tiles.size() - 2];
		xNeighbors = added.getExtendedNeighbors();
		empty = 0;
		for (uint32_t i = 0; i < xNeighbors.size(); i++) {
			if (!map.getRegion(xNeighbors[i]) && map.isInside(xNeighbors[i])) {
				empty++;
			}
		}
		if (empty > 0) {
			candidates.push_back(added);
		}

		//std::cout << " Candidates: " << candidates.size() << " Tiles " << tiles.size() << std::endl;

		goto election;
		
	}

	bool Corridor::isDeadEnd(Tile tile, Map& map) {
		std::array<Tile, 4> neighbors = tile.getNeighbors();
		int empty = 0;
		for (uint32_t i = 0; i < neighbors.size(); i++) {
			if (!map.getRegion(neighbors[i])) {
				++empty;
			}
		}
		return empty >= 3;
	}

	void Corridor::carveNext(Tile tile, Map& map) {
		std::array<Tile, 4> neighbors = tile.getNeighbors();
		std::array<Tile, 4> xNeighbors = tile.getExtendedNeighbors();
		std::vector<Tile> hops;
		std::vector<Tile> candidates;
		for (uint32_t i = 0; i < xNeighbors.size(); i++) {
			if (!map.getRegion(xNeighbors[i]) && map.isInside(xNeighbors[i])) {
				std::array<Tile, 4> n = xNeighbors[i].getNeighbors();
				int empty = 0;
				for (uint32_t j = 0; j < n.size(); j++) {
					if (!map.getRegion(n[j])) {
						empty++;
					}
				}
				if (empty != 4) {
					continue;
				}
				hops.push_back(neighbors[i]);
				candidates.push_back(xNeighbors[i]);
			}
		}
		if (candidates.empty()) {
			return;
		}

		std::uniform_int_distribution<int> directionDistribution(0, candidates.size() - 1);
		int index = directionDistribution(map.generator);
		tiles.push_back(candidates[index]);
		tiles.push_back(hops[index]);
		map.set(candidates[index], this);
		map.set(hops[index], this);
	}

	std::vector<Tile> Corridor::connectors(Map& map) {
		std::vector<Tile> connectors;
		for (auto tile : tiles) {
			std::array<Tile, 4> neighbors = tile.getNeighbors();
			std::array<Tile, 4> extendedNeighbors = tile.getExtendedNeighbors();
			for (uint32_t i = 0; i < extendedNeighbors.size(); i++) {
				auto region = map.getRegion(extendedNeighbors[i]);
				if (region && region.get() != this) {
					connectors.push_back(neighbors[i]);
				}
			}
		}
		return connectors;
	}

	void Corridor::shrink(float accuracy, Map& map) {
		int min = (int)(tiles.size() * accuracy);
		while (tiles.size() > min) {
			bool found = false;
			for (auto it = tiles.begin(); it != tiles.end();) {
				if (isDeadEnd(*it, map)) {
					found = true;
					it = tiles.erase(it);
				} else {
					++it;
				}
			}
			if (!found) {
				break;
			}
			map.update();
		}
	}
}