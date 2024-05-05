#include "ui.hh"	
#include <iostream>
#include <algorithm>

namespace UI {
	static uint32_t widget_count = 0; // widget counter used to assign unique IDs to each widget
	static std::vector<Widget*> widgets; 

	/* ======== Widget (Base class for UI elements) ======== */

	Widget::Widget(int x, int y, SDL_Renderer* &renderer):
		_rect({ x, y, 0, 0 }), // do not forget to set _rect w and h later!!
		_renderer(renderer),
		_id(widget_count++) {
		}

	void Widget::handleInput(SDL_Point mouse_pos, const uint8_t* kb_state){
		_mouse_over = SDL_PointInRect(&mouse_pos, &_rect);
		// kb_state not needed here but can be used in overriden functions if needed
	}

	uint32_t Widget::getID(){
		return _id;
	}

	bool removeWidget(uint32_t id){
		auto itr = std::remove_if(widgets.begin(), widgets.end(), [&](Widget* wg){
				return (wg->getID() == id);
				});
		if (itr == widgets.end()) return false;

		widgets.erase(itr, widgets.end());
		return true;
	}

	void renderAndHandleWidgets(SDL_Point mouse_pos, const uint8_t* kb_state){
		for (Widget* &wg : widgets){
			wg->render();
			wg->handleInput(mouse_pos, kb_state);
		}
	}

	/* ======== Text ======== */

	Text::Text(const std::string& text, 
			int x, int y, 
			SDL_Renderer* &renderer, 
			TTF_Font* font) : Widget(x, y, renderer) {
		widgets.push_back(this);
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
		widgets.push_back(this);

		_rect = { x, y, w, h };

		// calculate text size
		SDL_Rect t_sz = _text.getSize();
		int tw = t_sz.w, th = t_sz.h;
		// center text in the button
		_text.setPos(x + (w - tw) / 2, y + (h - th) / 2);
	}

	void Button::render(){
		SDL_Color c = (_mouse_over) ? _hover_color : _bg_color;
		SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
		SDL_RenderFillRect(_renderer, &_rect);
		_text.render();
	}
}
