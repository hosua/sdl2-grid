#pragma once

#include "scene.hh"
#include "defs.hh"
#include "color.hh"
#include "font.hh"

#include "ui/all.hh"

class Test : public IScene {
public:
	~Test() = default;
	Test(SDL_Renderer* &renderer);

	bool render(SDL_Renderer* &renderer) override;
	void handleInputs(SDL_Point& mouse_pos) override;

private:
	bool _end_game = false;
};
