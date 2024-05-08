#include "ui.hh"	
#include <algorithm>
#include <iostream>

namespace UI {
	static uint32_t s_widget_counter = 0; // assigns unique ID to each widget

	/* ======== IWidget (Base class for UI elements) ======== */

	IWidget::IWidget(int x, int y, SDL_Renderer* &renderer):
		_rect({ x, y, 0, 0 }), // do not forget to set _rect w and h later!!
		_renderer(renderer),
		_id(s_widget_counter++) {
		}

	void IWidget::handleInputs(SDL_Event event){
		if (isMouseOver() && isClicked(event))
			std::cout << "IWidget has no input action\n";
	}

	bool IWidget::isMouseOver() const {
		SDL_Point mouse_pos;
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		return SDL_PointInRect(&mouse_pos, &_rect);
	}

	bool IWidget::isMouseScrolledUp(SDL_Event event) const {
		return (isMouseOver() && 
				event.type == SDL_MOUSEWHEEL && event.wheel.y > 0);
	}

	bool IWidget::isMouseScrolledDown(SDL_Event event) const {
		return (isMouseOver() 
				&& event.type == SDL_MOUSEWHEEL && event.wheel.y < 0);
	}

	bool IWidget::isClicked(SDL_Event event) const {
		return (isMouseOver() 
				&& event.type == SDL_MOUSEBUTTONDOWN &&
				event.button.button == SDL_BUTTON_LEFT);
	}

	uint32_t IWidget::getID() const {
		return _id;
	}

	WidgetManager::WidgetManager(){}

	bool WidgetManager::addWidget(std::unique_ptr<IWidget> widget){ // true if add sucessful
		_widgets.push_back(std::move(widget));	
		return true;
	} 

	bool WidgetManager::removeWidget(uint32_t id){ // true if remove successful
		auto itr = std::remove_if(_widgets.begin(), _widgets.end(), [&](std::unique_ptr<IWidget>& wg){
				return (wg->getID() == id);
				});
		if (itr == _widgets.end()) return false;

		_widgets.erase(itr, _widgets.end());
		return true;
	} 

	void WidgetManager::renderWidgets(){
		for (std::unique_ptr<IWidget>& wg : _widgets)
			wg->render();
	}

	void WidgetManager::handleWidgetInputs(SDL_Event event){
		for (std::unique_ptr<IWidget>& wg : _widgets)
			wg->handleInputs(event);
	}


	/* ======== Text ======== */

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

	/* ======== Button ======== */

	Button::Button(const std::string& text,
			int x, int y,
			int w, int h,
			SDL_Renderer* &renderer,
			TTF_Font* font,
			SDL_Color bg_color,
			SDL_Color hover_color)
		: IWidget(x, y, renderer),
		_text(text, 0, 0, renderer, font),
		_bg_color(bg_color),
		_hover_color(hover_color) {
			_rect = { x, y, w, h };

			// calculate text size
			SDL_Rect t_sz = _text.getSize();
			int tw = t_sz.w, th = t_sz.h;
			// center text in the button
			_text.setPos(x + (w - tw) / 2, y + (h - th) / 2);
		}

	void Button::render(){
		SDL_Point mouse_pos;
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		SDL_Color c = (isMouseOver()) ? _hover_color : _bg_color;
		SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
		SDL_RenderFillRect(_renderer, &_rect);
		_text.render();
	}
	
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
