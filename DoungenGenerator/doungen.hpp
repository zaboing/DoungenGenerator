#ifndef DOUNGEN_HPP
#define	DOUNGEN_HPP

#include <vector>
#include <memory>
#include <array>
#include <random>
#include <ctime>
#include <iostream>


namespace doungen {

	class Map;

	struct handle {
	public:
		int id;
		handle(int i) : id(i) {}
		bool operator ==(const handle& h) const {
			return h.id == id;
		}
	};

	struct Tile {
		int x, y;
	public:
		Tile() : x(0), y(0) {}
		Tile(int xPos, int yPos): x(xPos), y(yPos) {}
		std::array<Tile, 4> getNeighbors();
		std::array<Tile, 4> getExtendedNeighbors();
		bool operator ==(const Tile& tile) const;

	};

	class Region {
	public:
		handle handle;
		Region() : handle(0) {}
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
	private:
		int width, height;
		int sequ;
		std::shared_ptr<Region> getRegion(Region* region);
		bool shouldGenerateCorridor(int x, int y);
	public:
		std::shared_ptr<Region>** tileMap;
		std::default_random_engine generator;
		std::vector<std::shared_ptr<Region>> regions;
		Map(int w, int h) : width(w), height(h), sequ(0) {
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
}

#endif	/* DOUNGEN_HPP */
