#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_W 1280
#define WINDOW_H 720

#define LEFT_PANE_W 140

#define BLOCK_W 40
#define BLOCK_H 40

#define PLAYER_MOVE_DELAY 3

#define FONT_OPENSANS "./assets/fonts/OpenSans-Regular.ttf"

extern SDL_Point POS_ZERO;

namespace Color {
	extern SDL_Color BLACK;
	extern SDL_Color BLUE;
	extern SDL_Color GREY;
	extern SDL_Color GREEN;
	extern SDL_Color LIGHT_GREY;
	extern SDL_Color RED;
	extern SDL_Color WHITE; 
}

namespace Font {
	extern TTF_Font* openSansTiny;
	extern TTF_Font* openSansSmall;
	extern TTF_Font* openSansMedium;
	extern TTF_Font* openSansLarge;

	void init();
	void closeAll();
}

typedef void (*func)(void*);
typedef void (*func_params); 

