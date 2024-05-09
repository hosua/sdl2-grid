#include "main_menu.hh"

#include <iostream>

#include "ui/all.hh"

#include "app.hh"

MainMenu::MainMenu():
	IScene("MAIN_MENU")
{

	std::unique_ptr<MainMenuWidgets::StartBtn> start_btn = 
		std::make_unique<MainMenuWidgets::StartBtn>();
	addWidget(std::move(start_btn));

	std::unique_ptr<MainMenuWidgets::TestBtn> test_btn = 
		std::make_unique<MainMenuWidgets::TestBtn>();
	addWidget(std::move(test_btn));

	std::unique_ptr<MainMenuWidgets::ExitBtn> exit_btn = 
		std::make_unique<MainMenuWidgets::ExitBtn>();
	addWidget(std::move(exit_btn));
};

void MainMenu::render() {
	renderWidgets();
};

void MainMenu::handleInputs(){
	const SDL_Point& mouse_pos = App::getInstance()->getMousePos();
	static bool lmb_down = false, rmb_down = false;
	for (const SDL_Event& event : App::getInstance()->getFrameEvents()){
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

	StartBtn::StartBtn():
		Button("Start", 
				(WINDOW_W/2) - (BTN_DIMS.w/2), (WINDOW_H / 3), 
				200, 50){}

	void StartBtn::handleInputs() {
		if (isMouseOver() && isClicked()){
			App::getInstance()->switchScene("GAME");
		}
	}

	TestBtn::TestBtn():
		Button("Test Menu",
				(WINDOW_W/2) - (BTN_DIMS.w/2), (WINDOW_H/3) + BTN_DIMS.h+5, 
				200, 50){}


	void TestBtn::handleInputs(){
		if (isMouseOver() && isClicked()){
			App::getInstance()->SceneManager::switchScene("TEST_MENU");
		}
	}

	ExitBtn::ExitBtn():
		Button("Exit",
				(WINDOW_W/2) - (BTN_DIMS.w/2), (WINDOW_H/3) + 2*(BTN_DIMS.h+5), 
				200, 50,
				Font::openSansMedium,
				Color::RED){}

	void ExitBtn::handleInputs() {
		if (isMouseOver() && isClicked()){
			std::cout << "Exiting the game.\n";
			App::getInstance()->isRunning();
		}
	}	
}
