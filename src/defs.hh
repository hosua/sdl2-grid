#pragma once
#include <SDL2/SDL.h>
#include <vector>

#include "color.hh"
#include "font.hh"

#define WINDOW_W 1280
#define WINDOW_H 720

#define LEFT_PANE_W 140

#define BLOCK_W 20
#define BLOCK_H 20

#define PLAYER_MOVE_DELAY 3

extern std::vector<SDL_Event>& GetFrameEvents();
extern const SDL_Point& GetMousePos();
