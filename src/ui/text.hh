#pragma once

#include "widget.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

namespace UI {
	class Text : public IWidget {
		public:
			~Text();
			Text(const std::string& text, 
					int x, int y, 
					SDL_Renderer* &renderer,
					TTF_Font* font
				);

			SDL_Rect getSize();
			void setPos(int x, int y);
			void setText(const std::string& text);
			void render() override;
		private:
			std::string _text;	
			SDL_Surface* _surface;
			SDL_Texture* _texture;
			TTF_Font* _font;
			SDL_Renderer* &_renderer;
	};

}
