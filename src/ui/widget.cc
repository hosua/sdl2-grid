#include "widget.hh"

#include <iostream>

#include "../defs.hh"

static uint32_t s_widget_counter = 0; // for assigning unique ID to each widget

namespace UI {

	/* ======== IWidget (Base class for UI elements) ======== */

	IWidget::IWidget(int x, int y, SDL_Renderer* &renderer):
		_rect({ x, y, 0, 0 }), // do not forget to set _rect w and h later!!
		_renderer(renderer),
		_id(s_widget_counter++) {}

	void IWidget::handleInputs(){
		if (isMouseOver() && isClicked())
			std::cout << "Widget has no input event!\n";
	}

	bool IWidget::isMouseOver() const {
		SDL_Point mouse_pos;
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		return SDL_PointInRect(&mouse_pos, &_rect);
	}

	bool IWidget::isMouseScrolledUp() const {
		for (const SDL_Event& event : GetFrameEvents())
			if (isMouseOver() && event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
				return true;
		return false;
	}

	bool IWidget::isMouseScrolledDown() const {
		for (const SDL_Event& event : GetFrameEvents())
			if (isMouseOver() && event.type == SDL_MOUSEWHEEL && event.wheel.y < 0)
				return true;
		return false;
	}

	bool IWidget::isClicked() const {
		for (const SDL_Event& event : GetFrameEvents())
			if (isMouseOver() && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				return true;
		return false;
	}

	uint32_t IWidget::getID() const {
		return _id;
	}

}
