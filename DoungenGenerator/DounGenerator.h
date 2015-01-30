#ifndef DOUNGENERATOR_H
#define DOUNGENERATOR_H

#include "doungen.hpp"

#include <vcclr.h>
#using <System.dll>

using namespace doungen;

namespace Doungen {
	public ref class MapModifier {
	public:
		int seed;
		int roomAttempts;
		float shrinkFactor;
	};

	public ref class ManagedMap {
		Map* impl;
	public:
		ManagedMap(int width, int height);
		void generate(MapModifier^ modifier);
		array<bool, 2>^ getMap();
		~ManagedMap();
	protected:
		!ManagedMap();
	};
}

#endif /* DOUNGENERATOR_H */