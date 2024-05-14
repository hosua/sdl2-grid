
#include "defs.hh"
#include "text.hh"

#include "app.hh"

namespace UI {
	Text::Text(const std::string& text, 
			int x, int y, 
			int w,
			TTF_Font* font) : 
		IWidget(x, y),
		_text_width(w),
		_text(text),
		_font(font){
			// we'll crash if we try to render an empty string, in that case
			// we'll just make the empty string a space so that this doesn't
			// happen. 
			//
			// No warning goes here since this may actually have a use
			// case (at least, while we only have a single Button class.
			//
			// Later, I plan on adding different types of buttons that won't
			// use text. Then this should be rewritten
			if (_text.empty()){
				_text = " ";
				// Wrapped not be able to return a surface if _text is empty or
				// contains only spaces, so we need to use this if it's empty.
			 	_surface = TTF_RenderText_Blended(font, _text.c_str(), Color::WHITE);
			} else {
				_surface = TTF_RenderText_Blended_Wrapped(_font, text.c_str(), Color::WHITE, _text_width );
			}
			if (!_surface){
				fprintf(stderr, "Error : %s", TTF_GetError());
			}
			_texture = SDL_CreateTextureFromSurface(App::getInstance()->getRenderer(), _surface);
			_rect = { x, y, _surface->w, _surface->h };
		}

	void Text::render(){
		SDL_RenderCopy(App::getInstance()->getRenderer(), _texture, nullptr, &_rect);
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
		if (_text.empty()){
			_text = " ";
			// Wrapped not be able to return a surface if _text is empty or
			// contains only spaces, so we need to use this if it's empty.
			_surface = TTF_RenderText_Blended(_font, _text.c_str(), Color::WHITE);
		} else {
			_surface = TTF_RenderText_Blended_Wrapped(_font, text.c_str(), Color::WHITE, _text_width );
		}
		if (!_surface){
			fprintf(stderr, "Error : %s", TTF_GetError());
		}
		_texture = SDL_CreateTextureFromSurface(App::getInstance()->getRenderer(), _surface);
		_rect = { _rect.x, _rect.y, _surface->w, _surface->h };
	}

}
