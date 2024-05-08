#pragma once

#include <SDL2/SDL.h>

namespace Color {
	extern SDL_Color BLACK;
	extern SDL_Color BLUE;
	extern SDL_Color GREY;
	extern SDL_Color GREEN;
	extern SDL_Color RED;
	extern SDL_Color WHITE; 
	extern SDL_Color YELLOW;

	namespace Light {
		extern SDL_Color BLUE;
		extern SDL_Color GREY;
		extern SDL_Color GREEN;
	}

	namespace VeryLight {
		extern SDL_Color GREY;
	}

	namespace Dark {
		extern SDL_Color GREY;
	}
}
