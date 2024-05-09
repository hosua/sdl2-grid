#pragma once

#include <SDL2/SDL.h>
#include "world.hh"

namespace PathFinder {

	std::vector<SDL_Point> bfs(World& world, const int& search_speed, SDL_Renderer* &renderer);

}
