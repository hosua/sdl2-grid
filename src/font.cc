#include "font.hh"

#include <iostream>

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

	void close(){
		TTF_CloseFont(openSansTiny);
		TTF_CloseFont(openSansSmall);
		TTF_CloseFont(openSansMedium);
		TTF_CloseFont(openSansLarge);
	}
}



