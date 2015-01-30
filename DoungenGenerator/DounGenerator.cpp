#include "DounGenerator.h"

using namespace Doungen;

void ManagedMap::generate(MapModifier^ modifier) {
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

ManagedMap::ManagedMap(int width, int height) {
	impl = new Map(width, height);
}

array<bool, 2>^ ManagedMap::getMap() {
	array<bool, 2>^ map = gcnew array<bool, 2>(impl->width, impl->height);

	for (int i = impl->width - 1; i >= 0; --i) {
		for (int j = impl->height - 1; j >= 0; --j) {
			auto region = impl->getRegion(i, j);
			map[i, j] = region ? true : false;
		}
	}

	return map;
}

ManagedMap::~ManagedMap() {
	delete impl;
}

ManagedMap::!ManagedMap() {
	delete impl;
}

