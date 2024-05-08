#include "color.hh"

namespace Color {
	SDL_Color BLACK = { 0, 0, 0, 255 };
	SDL_Color BLUE = { 0, 87, 201, 255 };
	SDL_Color GREEN = { 32, 148, 0, 255 };
	SDL_Color GREY = { 105, 105, 105, 255 };
	SDL_Color RED = { 171, 11, 0, 255 };
	SDL_Color WHITE = { 255, 255, 255, 255 };
	
	namespace Light {
		SDL_Color BLUE = { 19, 176, 255, 255 };
		SDL_Color GREEN = { 0, 226, 0, 255 };
		SDL_Color GREY = { 150, 150, 150, 255 };
	}
	
	namespace VeryLight {
		SDL_Color GREY = { 200, 200, 200, 255 };
	}

	namespace Dark {
		SDL_Color GREY = { 64, 64, 64, 255 };
	}
}
