#include "spinner.hh"
#include <algorithm>

namespace UI {
	template<typename T>
	Spinner<T>::Spinner(T& val,
			int x, int y,
			int w, int h,
			T min_val, T max_val, T interval,
			SDL_Renderer* &renderer,
			SpinnerType spinner_type,
			TTF_Font* font,
			SDL_Color bg_color,
			SDL_Color btn_color,
			SDL_Color btn_hover_color)
		: IWidget(x, y, renderer), 
		_text(std::to_string(val), 0, 0, renderer, font),
		_val(val), _min_val(min_val), _max_val(max_val), _interval(interval),
		_bg_color(bg_color), _btn_color(btn_color), _btn_hover_color(btn_hover_color),
		_type(spinner_type) {
			static_assert(std::is_arithmetic<T>::value, "Error: UI::Spinner template must be numeric type.");
			_rect = { x, y, w, h };		
			if (_type == ST_VERTICAL){
				// vertical configuration
				_inc_btn = std::make_unique<Button>(
						"+",
						x, y,
						w, h/3,
						renderer, 
						font);

				_dec_btn = std::make_unique<Button>(
						"-",
						x, y + 2*h/3+1,
						w, h/3,
						renderer, 
						font);
			} else { 
				// horizontal configuration
				_inc_btn = std::make_unique<Button>(
						"+",
						x + 2*w/3+1, y,
						w/3, h,
						renderer, 
						font);

				_dec_btn = std::make_unique<Button>(
						"-",
						x, y,
						w/3, h,
						renderer, 
						font);
				
			}

			SDL_Rect t_sz = _text.getSize();
			_text.setPos(x + (w/2) - (t_sz.w/2), y + (h/2) - (t_sz.h/2));
		}

	// Note: I do not understand why, but it is necessary to specify these
	// templates when overriding virtual methods.
	template class UI::Spinner<short>;
	template class UI::Spinner<unsigned short>;
	template class UI::Spinner<int>;
	template class UI::Spinner<unsigned int>;
	template class UI::Spinner<long>;
	template class UI::Spinner<unsigned long>;
	template class UI::Spinner<float>;
	template class UI::Spinner<double>;

	template<typename T>
	SDL_Rect Spinner<T>::getSize(){
		return _rect;
	}

	template<typename T>
	void Spinner<T>::setPos(int x, int y){
		_rect.x = x, _rect.y = y;
	}

	template<typename T>
	void Spinner<T>::incVal(){
		_val = std::clamp<int>(_val + _interval, _min_val, _max_val);
		_text.setText(std::to_string(_val));
		// re-center the text
		SDL_Rect t_sz = _text.getSize();
		_text.setPos(_rect.x + (_rect.w/2) - (t_sz.w/2), _rect.y + (_rect.h/2) - (t_sz.h/2));
	}

	template<typename T>
	void Spinner<T>::decVal(){
		_val = std::clamp<T>(_val - _interval, _min_val, _max_val);
		_text.setText(std::to_string(_val));
		// re-center the text
		SDL_Rect t_sz = _text.getSize();
		_text.setPos(_rect.x + (_rect.w/2) - (t_sz.w/2), _rect.y + (_rect.h/2) - (t_sz.h/2));
	}


	template<typename T>
	void Spinner<T>::render(){
		SDL_Color c = _bg_color;
		SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, 255);
		SDL_RenderFillRect(_renderer, &_rect);

		_inc_btn->render();
		_text.render();
		_dec_btn->render();
	}

	template<typename T>
	void Spinner<T>::handleInputs(SDL_Event event){
		IWidget::handleInputs(event);
		if (isMouseScrolledUp(event) || _inc_btn->isClicked(event))
			incVal();
		if (isMouseScrolledDown(event) || _dec_btn->isClicked(event))
			decVal();
	}

}

