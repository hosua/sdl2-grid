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

// Gets a global vector of SDL_Events, which are polled in App::mainLoop().
// This is necessary to handle events outside of while(SDL_PollEvents()).
extern std::vector<SDL_Event>& GetFrameEvents();
extern const SDL_Point& GetMousePos();

// Stops the current thread from running for float (ms) time. Converted to
// ns for higher resolution.
extern void DelayHighRes(float ms);

