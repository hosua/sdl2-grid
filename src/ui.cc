#include "ui.hh"	
#include <algorithm>
#include <iostream>

namespace UI {
	static uint32_t s_widget_counter = 0; // assigns unique ID to each widget

	/* ======== Widget (Base class for UI elements) ======== */

	Widget::Widget(int x, int y, SDL_Renderer* &renderer):
		_rect({ x, y, 0, 0 }), // do not forget to set _rect w and h later!!
		_renderer(renderer),
		_id(s_widget_counter++) {
	}

	void Widget::handleInputs(SDL_Event event){
		if (isMouseOver() &&
				event.type == SDL_MOUSEBUTTONDOWN &&
				event.button.button == SDL_BUTTON_LEFT){
			std::cout << "Widget has no input action\n";
		}
	}

	bool Widget::isMouseOver(){
		SDL_Point mouse_pos;
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		return SDL_PointInRect(&mouse_pos, &_rect);
	}

	uint32_t Widget::getID(){
		return _id;
	}

	WidgetManager::WidgetManager(){
	}
	
	bool WidgetManager::addWidget(std::unique_ptr<Widget> widget){ // true if add sucessful
		_widgets.push_back(std::move(widget));	
		return true;
	} 

	bool WidgetManager::removeWidget(uint32_t id){ // true if remove successful
		auto itr = std::remove_if(_widgets.begin(), _widgets.end(), [&](std::unique_ptr<Widget>& wg){
				return (wg->getID() == id);
				});
		if (itr == _widgets.end()) return false;

		_widgets.erase(itr, _widgets.end());
		return true;
	} 
	
	void WidgetManager::renderWidgets(){
		for (std::unique_ptr<Widget>& wg : _widgets)
			wg->render();
	}

	void WidgetManager::handleWidgetInputs(SDL_Event event){
		for (std::unique_ptr<Widget>& wg : _widgets)
			wg->handleInputs(event);
	}


	/* ======== Text ======== */

	Text::Text(const std::string& text, 
			int x, int y, 
			SDL_Renderer* &renderer, 
			TTF_Font* font) : Widget(x, y, renderer) {
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

	/* ======== Button ======== */

	Button::Button(const std::string& text,
			int x, int y,
			int w, int h,
			SDL_Renderer* &renderer,
			TTF_Font* font,
			SDL_Color bg_color,
			SDL_Color hover_color)
	: Widget(x, y, renderer),
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
			SDL_Renderer* &renderer,
			int min_val, int max_val,
			TTF_Font* font,
			SDL_Color bg_color,
			SDL_Color hover_color)
	: Widget(x, y, renderer), 
	_text("0", 0, 0, renderer, font),
	_val(val), _min_val(min_val), _max_val(max_val) {}

	SDL_Rect Spinner::getSize(){
		return _rect;
	}

	void Spinner::setPos(int x, int y){
		_rect.x = x, _rect.y = y;
	}

	void Spinner::incVal(){
		_val = std::clamp<int>(++_val, _min_val, _max_val);
	}

	void Spinner::decVal(){
		_val = std::clamp<int>(--_val, _min_val, _max_val);
	}
}
