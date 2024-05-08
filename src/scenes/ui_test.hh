#pragma once

#include "scene.hh"
#include "defs.hh"
#include "color.hh"
#include "font.hh"

#include "ui/all.hh"

class TestUI : public IScene {
public:
	~TestUI() = default;
	TestUI(SDL_Renderer* &renderer);

	bool render(SDL_Renderer* &renderer) override;
	void handleInputs(SDL_Point& mouse_pos) override;

private:
	bool _end_game = false;
};
