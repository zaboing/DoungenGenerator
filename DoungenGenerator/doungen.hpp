#ifndef DOUNGEN_HPP
#define	DOUNGEN_HPP

#include <vector>
#include <memory>
#include <map>
#include <array>
#include <random>

namespace doungen {
    
	class Map;

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
		int width, height;
	public:
		std::shared_ptr<Region>** tileMap;
		std::default_random_engine generator;
		std::vector<std::shared_ptr<Region>> regions;
		Map(int w, int h) : width(w), height(h) {
			generator.seed(112);
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
		void generateCorridor(int startX, int startY);
		void shrinkCorridors(float accuracy);
		void update();
	};
}

#endif	/* DOUNGEN_HPP */

