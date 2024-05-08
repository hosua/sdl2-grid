#pragma once

#include <unordered_map>

namespace PathFinder {
	// <speed_val, delay (ms)>
	extern std::unordered_map<int, float> SEARCH_SPEED_MAP;
	
	// TODO: This doesn't really belong here, but it's the only place I'm
	// currently using it. Maybe move this somewhere in the engine if we need
	// it again in the future. Maybe we should move it to global defs later?
	
	// Stops the current thread from running for float (ms) time. Converted to
	// ns for higher resolution.
	void delayHighRes(float ms);
}
