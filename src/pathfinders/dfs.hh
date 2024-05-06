#pragma once

#include <SDL2/SDL.h>
#include "../world.hh"

std::vector<SDL_Point> dfs(World& world, SDL_Renderer* &renderer);
