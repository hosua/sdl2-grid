#pragma once

#include <SDL2/SDL.h>
#include "../world.hh"

namespace PathFinder {
	std::vector<SDL_Point> dfs(World& world, const int& search_speed, std::vector<SDL_Rect>& search_markers);

}
