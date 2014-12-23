#include "doungen.hpp"

namespace doungen {
	bool Region::operator ==(Region* region) {
		return region->tiles == tiles;
	}
}