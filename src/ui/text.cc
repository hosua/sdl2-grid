
#include "defs.hh"
#include "font.hh"
#include "color.hh"
#include "text.hh"

namespace UI {
	Text::Text(const std::string& text, 
			int x, int y, 
			SDL_Renderer* &renderer, 
			TTF_Font* font) : 
		IWidget(x, y, renderer),
		_text(text),
		_font(font),
		_renderer(renderer){
			// we'll crash if we try to render an empty string, in that case
			// we'll just make the empty string a space so that this doesn't
			// happen. 
			//
			// No warning goes here since this may actually have a use
			// case (at least, while we only have a single Button class.
			//
			// Later, I plan on adding different types of buttons that won't
			// use text. Then this should be rewrittenV
			if (_text.empty()) _text = " ";
			
			_surface = TTF_RenderText_Blended(font, _text.c_str(), Color::WHITE);
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
		_text = text;
		// once again, avoid the empty string by inserting a space
		if (_text.empty()) _text = " ";

		_surface = TTF_RenderText_Blended(_font, text.c_str(), Color::WHITE);
		_texture = SDL_CreateTextureFromSurface(_renderer, _surface);
		_rect = { _rect.x, _rect.y, _surface->w, _surface->h };
	}

}
