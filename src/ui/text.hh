#pragma once

#include "widget.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

namespace UI {
	/* TODO: We need to support line wrapping!!!
	 * See: https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_Solid_Wrapped
	 */
	class Text : public IWidget {
		public:
			~Text();
			Text(const std::string& text, 
					int x, int y, 
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
	};

}
