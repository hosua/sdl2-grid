#include "defs.hh"

#include <iostream>

SDL_Point POS_ZERO = { 0, 0 };

namespace Color {
	SDL_Color BLACK = { 0, 0, 0, 255 };
	SDL_Color BLUE = { 0, 87, 201, 255 };
	SDL_Color LIGHT_BLUE = { 19, 176, 255, 255 };
	SDL_Color GREEN = { 32, 148, 0, 255 };
	SDL_Color LIGHT_GREEN = { 0, 226, 0, 255 };
	SDL_Color DARK_GREY = { 64, 64, 64, 255 };
	SDL_Color GREY = { 105, 105, 105, 255 };
	SDL_Color LIGHT_GREY = { 150, 150, 150, 255 };
	SDL_Color VERY_LIGHT_GREY = { 200, 200, 200, 255 };
	SDL_Color RED = { 171, 11, 0, 255 };
	SDL_Color WHITE = { 255, 255, 255, 255 };
}

namespace Font {
	TTF_Font* openSansTiny = nullptr;
	TTF_Font* openSansSmall = nullptr;
	TTF_Font* openSansMedium = nullptr;
	TTF_Font* openSansLarge = nullptr;
	
	bool init(){
		if ((openSansTiny = TTF_OpenFont(FONT_OPENSANS, 8)) == nullptr)
			std::cerr << SDL_GetError() << '\n';
		if ((openSansSmall = TTF_OpenFont(FONT_OPENSANS, 16)) == nullptr)
			std::cerr << SDL_GetError() << '\n';
		if ((openSansMedium = TTF_OpenFont(FONT_OPENSANS, 32)) == nullptr)
			std::cerr << SDL_GetError() << '\n';
		if ((openSansLarge = TTF_OpenFont(FONT_OPENSANS, 64)) == nullptr)
			std::cerr << SDL_GetError() << '\n';

		return (openSansTiny && openSansMedium && openSansMedium && openSansLarge);
	}

	void closeAll(){
		TTF_CloseFont(openSansTiny);
		TTF_CloseFont(openSansSmall);
		TTF_CloseFont(openSansMedium);
		TTF_CloseFont(openSansLarge);
	}
}


