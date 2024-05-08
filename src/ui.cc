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
		return (isMouseOver() && event.type == SDL_MOUSEWHEEL && event.wheel.y > 0);
	}

	bool IWidget::isMouseScrolledDown(SDL_Event event) const {
		return (isMouseOver() && event.type == SDL_MOUSEWHEEL && event.wheel.y < 0);
	}

	bool IWidget::isClicked(SDL_Event event) const {
		return (isMouseOver() && event.type == SDL_MOUSEBUTTONDOWN &&
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

	Spinner::Spinner(int& val,
			int x, int y,
			int w, int h,
			int min_val, int max_val,
			SDL_Renderer* &renderer,
			SpinnerType spinner_type,
			TTF_Font* font,
			SDL_Color bg_color,
			SDL_Color hover_color)
		: IWidget(x, y, renderer), 
		_text(std::to_string(val), 0, 0, renderer, font),
		_val(val), _min_val(min_val), _max_val(max_val),
		_bg_color(bg_color), _hover_color(hover_color),
		_type(spinner_type) {
			_rect = { x, y, w, h };		

			if (_type == ST_VERTICAL){
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
			} else { // horizontal configuration
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

	SDL_Rect Spinner::getSize(){
		return _rect;
	}

	void Spinner::setPos(int x, int y){
		_rect.x = x, _rect.y = y;
	}

	void Spinner::incVal(){
		_val = std::clamp<int>(++_val, _min_val, _max_val);
		_text.setText(std::to_string(_val));
		// re-center the text
		SDL_Rect t_sz = _text.getSize();
		_text.setPos(_rect.x + (_rect.w/2) - (t_sz.w/2), _rect.y + (_rect.h/2) - (t_sz.h/2));
	}

	void Spinner::decVal(){
		_val = std::clamp<int>(--_val, _min_val, _max_val);
		_text.setText(std::to_string(_val));
		// re-center the text
		SDL_Rect t_sz = _text.getSize();
		_text.setPos(_rect.x + (_rect.w/2) - (t_sz.w/2), _rect.y + (_rect.h/2) - (t_sz.h/2));
	}

	void Spinner::render(){
		SDL_Color c = _bg_color;
		SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, 255);
		SDL_RenderFillRect(_renderer, &_rect);

		_inc_btn->render();
		_text.render();
		_dec_btn->render();
	}

	void Spinner::handleInputs(SDL_Event event){
		if (isMouseScrolledUp(event) || _inc_btn->isClicked(event))
			incVal();
		if (isMouseScrolledDown(event) || _dec_btn->isClicked(event))
			decVal();
	}

}
