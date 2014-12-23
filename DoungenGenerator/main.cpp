#include "doungen.hpp"
#include <iostream>

void print(doungen::Map& map);

int main(int argc, const char* argv[]) {
	doungen::Map map(75, 70);
	map.generateRooms(90);
	map.generateCorridor(1, 1);
	print(map);
	return 0;
}

void print(doungen::Map& map) {
	std::cout << "Generated " << map.regions.size() << " regions." << std::endl;
	for (int j = 0; j < map.getHeight(); j++) {
		for (int i = 0; i < map.getWidth(); i++) {
			std::shared_ptr<doungen::Region> region = map.getRegion(i, j);
			if (region) {
				std::cout << " ";
			} else {
				std::cout << "x";
			}
		}
		std::cout << std::endl;
	}
}