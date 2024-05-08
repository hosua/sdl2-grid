#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>

#include "defs.hh"
#include "widget.hh"
#include "text.hh"
#include "button.hh"

namespace UI {
	typedef enum {
		ST_VERTICAL,
		ST_HORIZONTAL,
	} SpinnerType;


	// TODO: I really want to be able to support all arithmetic types (uint8_t,
	// short, long, etc). However, I can't get the implementation to work. For
	// now, casting any of these types should work fine but look into this
	// again later.
	template<typename T>
	class Spinner : public IWidget {
		public:
			~Spinner() = default;
			Spinner(T& val,
					int x, int y,
					int w, int h,
					T min_val, T max_val, T interval,
					SDL_Renderer* &renderer,
					SpinnerType spinner_type,
					TTF_Font* font = Font::openSansSmall,
					SDL_Color bg_color = Color::GREY,
					SDL_Color btn_color = Color::DARK_GREY,
					SDL_Color btn_hover_color = Color::LIGHT_GREY);

			SDL_Rect getSize();
			void setPos(int x, int y);
			void incVal();
			void decVal();

			void render() override;
			void handleInputs(SDL_Event event) override;
			
		private:
			Text _text;
			T _val, _min_val, _max_val, _interval;
			SDL_Color _bg_color, _btn_color, _btn_hover_color;
			std::unique_ptr<Button> _inc_btn, _dec_btn;
			SpinnerType _type; 
	};
}

