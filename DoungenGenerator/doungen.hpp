#ifndef DOUNGEN_HPP
#define	DOUNGEN_HPP

#include <vcclr.h>

#include <vector>
#include <memory>
#include <array>
#include <random>
#include <ctime>
#include <iostream>

#using <System.dll>

namespace doungen {

	class Map;

	struct Tile {
		int x, y;
		Tile() : x(0), y(0) {}
		Tile(int xPos, int yPos): x(xPos), y(yPos) {}
		std::array<Tile, 4> getNeighbors();
		std::array<Tile, 4> getExtendedNeighbors();
		bool operator ==(const Tile& tile) const;

	};

	class Region {
	public:
		std::vector<Tile> tiles;
		virtual std::vector<Tile> connectors(Map& map) = 0;
		bool operator ==(Region* region);
	};
    
	class Room : public Region {
	public:
		int x, y, width, height;
		Room(int xPos, int yPos, int w, int h);
		std::vector<Tile> connectors(Map& map);
		bool intersects(std::shared_ptr<Region> region);
		void shrink(int amountX, int amountY);
		void applyTiles();
	};
    
	class Corridor : public Region {
	private:
		bool isDeadEnd(Tile tile, Map& map);
		void carveNext(Tile tile, Map& map);
		void generate(Map& map, std::vector<Tile>& tiles);
	public:
		Corridor(Tile start);
		Corridor(int startX, int startY);
		std::vector<Tile> connectors(Map& map);
		void generate(Map& map);
		void shrink(float accuracy, Map& map);
	};

	class Map {
	public:
		int width, height;
		std::shared_ptr<Region> getRegion(Region* region);
		bool shouldGenerateCorridor(int x, int y);

		std::shared_ptr<Region>** tileMap;
		std::default_random_engine generator;
		std::vector<std::shared_ptr<Region>> regions;
		Map(int w, int h) : width(w), height(h) {
			tileMap = new std::shared_ptr<Region>*[width];
			for (int i = 0; i < width; i++) {
				tileMap[i]= new std::shared_ptr<Region>[height];
			}
		}
		~Map() { delete[] tileMap; }
		int getWidth() const { return width; }
		int getHeight() const { return height; }
		std::shared_ptr<Region> getRegion(Tile position) {
			return getRegion(position.x, position.y);
		}
		std::shared_ptr<Region> getRegion(int x, int y);
		bool isInside(int x, int y) const;
		bool isInside(Tile tile) const;
		void generateRooms(int attempts);
		void generateCorridors();
		void generateCorridor(int startX, int startY);
		void shrinkCorridors(float accuracy);
		void update();
		void unset(std::vector<Tile>& tiles);
		void unset(Tile& tile);
		void set(std::vector<Tile>& tiles, Region* region);
		void set(std::vector<Tile>& tiles, std::shared_ptr<Region> region);
		void set(Tile& tile, Region* region);
		void set(Tile& tile, std::shared_ptr<Region> region);
	};

	/*template<typename T, int rank = 1>
        ref class array : System::Array {};*/

	public ref class MapModifier {
	public:
		int seed;
		int roomAttempts;
		float shrinkFactor;
	};

	public ref class ManagedMap {
		Map* impl;
	public:
		ManagedMap(int width, int height) { impl = new Map(width, height); }
		void generate(MapModifier^ modifier) {
			impl->generator.seed(modifier->seed);
			impl->generateRooms(modifier->roomAttempts);
			impl->generateCorridors();
			
			for (auto region : impl->regions) {
				std::shared_ptr<Room> room = std::dynamic_pointer_cast<Room>(region);
				if (room) {
					room->connectors(*impl);
				}
			}

			impl->shrinkCorridors(1 - modifier->shrinkFactor);
		}
		array<bool, 2>^ getMap() {
			array<bool, 2>^ map = gcnew array<bool, 2>(impl->width, impl->height);

			for (int i = impl->width - 1; i >= 0; --i) {
				for (int j = impl->height - 1; j >= 0; --j) {
					auto region = impl->getRegion(i, j);
					map[i, j] = region ? true : false;
				}
			}

			return map;
		}
		~ManagedMap() { delete impl; }
	protected:
		!ManagedMap() { delete impl; }
	};
}

extern "C" 

#endif	/* DOUNGEN_HPP */
