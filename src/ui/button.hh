#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

#include "font.hh"
#include "color.hh"
#include "widget.hh"
#include "text.hh"

namespace UI {
class Button : public IWidget {
		public:
			Button(const std::string& text, 
					int x, int y, 
					int w, int h,
					SDL_Renderer* &renderer,
					TTF_Font* font = Font::openSansMedium,
					SDL_Color bg_color = Color::Dark::GREY,
					SDL_Color hover_color = Color::Light::GREY
				  );

			~Button() = default;
			
			// override handleInputs from IWidget class for input events
			// void handleInput(SDL_Point mouse_pos, const uint8_t* kb_state) override;
			void render() override;
		private:
			Text _text;
			SDL_Color _bg_color, _hover_color;
	};

}
