#include "main_menu.hh"

#include <iostream>

#include "ui/all.hh"

#include "app.hh"

MainMenu::MainMenu(SDL_Renderer* &renderer, SceneManager &scene_mgr, bool& running):
	IScene("MAIN_MENU", renderer),
	_running(running)
{

	std::unique_ptr<MainMenuWidgets::StartBtn> start_btn = 
		std::make_unique<MainMenuWidgets::StartBtn>(renderer, scene_mgr);
	addWidget(std::move(start_btn));

	std::unique_ptr<MainMenuWidgets::TestBtn> test_btn = 
		std::make_unique<MainMenuWidgets::TestBtn>(renderer, scene_mgr);
	addWidget(std::move(test_btn));

	std::unique_ptr<MainMenuWidgets::ExitBtn> exit_btn = 
		std::make_unique<MainMenuWidgets::ExitBtn>(renderer);
	addWidget(std::move(exit_btn));
};

void MainMenu::render(SDL_Renderer* &renderer) {
	renderWidgets();
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
		if (isMouseOver() && isClicked()){
			_scene_mgr.switchScene("GAME");
		}
	}

	TestBtn::TestBtn(SDL_Renderer* &renderer, SceneManager& scene_mgr):
		Button("Test Menu",
				(WINDOW_W/2) - (BTN_DIMS.w/2), (WINDOW_H/3) + BTN_DIMS.h+5, 
				200, 50,
				renderer),
			_scene_mgr(scene_mgr) {}


	void TestBtn::handleInputs(){
		if (isMouseOver() && isClicked()){
			_scene_mgr.switchScene("TEST_MENU");
		}
	}

	ExitBtn::ExitBtn(SDL_Renderer* &renderer):
		Button("Exit",
				(WINDOW_W/2) - (BTN_DIMS.w/2), (WINDOW_H/3) + 2*(BTN_DIMS.h+5), 
				200, 50,
				renderer, 
				Font::openSansMedium,
				Color::RED){}

	void ExitBtn::handleInputs() {
		if (isMouseOver() && isClicked()){
			std::cout << "Exiting the game.\n";
			App::getInstance()->isRunning();
		}
	}	
}
