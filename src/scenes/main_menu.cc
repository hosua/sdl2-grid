#include "main_menu.hh"

#include <iostream>

#include "ui/all.hh"

MainMenu::MainMenu(SDL_Renderer* &renderer, SceneManager &scene_mgr, bool& running):
	IScene("MAIN_MENU", renderer)
{

	std::unique_ptr<MainMenuWidgets::StartBtn> start_btn = 
		std::make_unique<MainMenuWidgets::StartBtn>(renderer, scene_mgr);
	addWidget(std::move(start_btn));

	std::unique_ptr<MainMenuWidgets::ExitBtn> exit_btn = 
		std::make_unique<MainMenuWidgets::ExitBtn>(renderer, running);
	addWidget(std::move(exit_btn));
};

bool MainMenu::render(SDL_Renderer* &renderer) {
	renderWidgets();
	return (!_end_game);
};

void MainMenu::handleInputs(){
	const SDL_Point& mouse_pos = GetMousePos();
	static bool lmb_down = false, rmb_down = false;
	for (const SDL_Event& event : GetFrameEvents()){
		switch(event.type){
			// Handle input events here
			default:
				break;
		}
	}

	handleWidgetInputs();
}

namespace MainMenuWidgets {
	static const SDL_Rect BTN_DIMS = { 0, 0, 200, 50 };

	StartBtn::StartBtn(SDL_Renderer* &renderer, SceneManager &scene_manager):
		Button("Start", 
				(WINDOW_W/2) - (BTN_DIMS.w/2), (WINDOW_H / 3), 
				200, 50,
				renderer),
		_scene_mgr(scene_manager) {}

	void StartBtn::handleInputs() {
		if (isMouseOver() && isClicked())
			std::cout << "Clicked start button.\n";
	}

	ExitBtn::ExitBtn(SDL_Renderer* &renderer, bool& running):
			Button("Exit",
				(WINDOW_W/2) - (BTN_DIMS.w/2), (WINDOW_H/3) + BTN_DIMS.h+5, 
				200, 50,
				renderer, 
				Font::openSansSmall,
				Color::RED),
			_running(running) {}

	void ExitBtn::handleInputs() {
		if (isMouseOver() && isClicked()){
			std::cout << "Exiting the game.\n";
			_running = false;
		}
	}	
}
