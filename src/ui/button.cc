#include "button.hh"

#include "app.hh"

namespace UI {
	Button::Button(const std::string& text,
			int x, int y,
			int w, int h,
			TTF_Font* font,
			SDL_Color bg_color,
			SDL_Color hover_color)
		: IWidget(x, y),
		_text(text, 0, 0, 0, font), // TODO: wrapline == 0 means that it will only newline on \n
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
		SDL_Renderer* renderer = App::getInstance()->getRenderer();
		SDL_Point mouse_pos;
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		SDL_Color c = (isMouseOver()) ? _hover_color : _bg_color;
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		SDL_RenderFillRect(renderer, &_rect);
		_text.render();
	}
}
