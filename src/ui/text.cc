#include "text.hh"

#include "defs.hh"

namespace UI {
	Text::Text(const std::string& text, 
			int x, int y, 
			SDL_Renderer* &renderer, 
			TTF_Font* font) : 
		IWidget(x, y, renderer),
		_font(font),
		_renderer(renderer){
			_surface = TTF_RenderText_Blended(font, text.c_str(), Color::WHITE);
			_texture = SDL_CreateTextureFromSurface(renderer, _surface);
			_rect = { x, y, _surface->w, _surface->h };
		}

	void Text::render(){
		SDL_RenderCopy(_renderer, _texture, nullptr, &_rect);
	}

	Text::~Text(){
		SDL_FreeSurface(_surface);
		SDL_DestroyTexture(_texture);
	}

	SDL_Rect Text::getSize(){
		return { 0, 0, _surface->w, _surface->h };
	}

	void Text::setPos(int x, int y){
		_rect.x = x, _rect.y = y;
	}

	void Text::setText(const std::string& text){
		_surface = TTF_RenderText_Blended(_font, text.c_str(), Color::WHITE);
		_texture = SDL_CreateTextureFromSurface(_renderer, _surface);
		_rect = { _rect.x, _rect.y, _surface->w, _surface->h };
	}

}
