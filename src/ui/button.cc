#include "button.hh"

namespace UI {
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
}
