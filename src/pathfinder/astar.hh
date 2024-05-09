#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "world.hh"

namespace PathFinder {
	std::vector<SDL_Point> a_star(World& world);
}
