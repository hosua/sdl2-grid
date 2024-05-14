#pragma once

// TODO: Josh... just put this in world.hh

#include <SDL2/SDL.h>
#include <unordered_map>
#include "defs.hh"

typedef enum {
	ENT_NONE,
	ENT_END,
	ENT_WALL,
	ENT_PLAYER,
} EntType;

extern std::unordered_map<EntType, SDL_Color> ENT_COLOR_MAP;
