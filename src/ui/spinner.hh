#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>

#include "defs.hh"
#include "font.hh"
#include "color.hh"
#include "widget.hh"
#include "text.hh"
#include "button.hh"

// TODO: We might want to make this possible to define in the API.
// This value represents how many decimals places to render for spinners that
// use floats/doubles.
#define SPINNER_PRECISION 2

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
					SpinnerType spinner_type,
					float btn_scalar = .25, 
					TTF_Font* font = Font::openSansSmall,
					SDL_Color bg_color = Color::GREY,
					SDL_Color btn_color = Color::Dark::GREY,
					SDL_Color btn_hover_color = Color::Light::GREY);

			SDL_Rect getSize();
			void setPos(int x, int y);
			void incVal();
			void decVal();

			void render() override;
			void handleInputs() override;
			
		private:
			Text _text;
			T& _val, _min_val, _max_val, _interval;
			float _btn_scalar; // the relative size of the buttons in the spinner, should be a value between 0.0-1.0
			SDL_Color _bg_color, _btn_color, _btn_hover_color;
			std::unique_ptr<TextButton> _inc_btn, _dec_btn;
			SpinnerType _type; 
	};
}

