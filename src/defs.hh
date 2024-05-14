#pragma once
#include <SDL2/SDL.h>
#include <vector>

#include "color.hh"
#include "font.hh"

#define WINDOW_W 1280
#define WINDOW_H 720

struct Settings {
	int block_size = 10;
};

extern Settings g_settings;


