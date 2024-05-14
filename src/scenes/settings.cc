#include "settings.hh"

#include <iostream>

#include "ui/all.hh"
#include "ui/spinner.hh"

#include "app.hh"

int v_val = 0; 

static const int WORLD_X = 200, WORLD_Y = 200, WORLD_W = 400, WORLD_H = 400;

using namespace SettingsWidgets;

namespace Scenes {
	Settings::Settings():
		IScene("SETTINGS_MENU"),
		World(WORLD_X, WORLD_Y, WORLD_W, WORLD_H){

			// block size label
			std::unique_ptr<UI::Text> block_size_lbl =
				std::make_unique<UI::Text>("Block Size",
						110, 200,
						50,
						Font::openSansSmall);
			addWidget(std::move(block_size_lbl));

			// vertical spinner	
			std::unique_ptr<UI::Spinner<int>> v_spnr =
				std::make_unique<UI::Spinner<int>>(g_settings.block_size,
						160, 200,
						26, 65,
						1, 100, 1,
						UI::ST_VERTICAL
						);
			addWidget(std::move(v_spnr));	

			std::unique_ptr<MainMenuBtn> btn_main_menu = 
				std::make_unique<MainMenuBtn>();
			addWidget(std::move(btn_main_menu));

			std::unique_ptr<ExitBtn> btn_exit = 
				std::make_unique<ExitBtn>();
			addWidget(std::move(btn_exit));
		};

	void Settings::render(){
		World::updateDimensions();
		World::render();
		renderWidgets();
	};

	void Settings::handleInputs(){

		const SDL_Point& mouse_pos = App::getInstance()->getMousePos();
		static bool lmb_down = false, rmb_down = false;
		for (const SDL_Event& event : App::getInstance()->getFrameEvents()){
			switch(event.type){
				case SDL_MOUSEBUTTONDOWN:
					{
						if (event.button.button == SDL_BUTTON_LEFT)
							lmb_down = true;
						else if (event.button.button == SDL_BUTTON_RIGHT)
							rmb_down = true;
						break;
					}
				case SDL_MOUSEBUTTONUP:
					{
						if (event.button.button == SDL_BUTTON_LEFT)
							lmb_down = false;
						else if (event.button.button == SDL_BUTTON_RIGHT)
							rmb_down = false;
						break;
					}
				case SDL_KEYDOWN:
					{
						// if (event.key.keysym.scancode == SDL_SCANCODE_P)

						if (event.key.keysym.scancode == SDL_SCANCODE_1)
							setEntityType(PathFinder::ENT_WALL);
						if (event.key.keysym.scancode == SDL_SCANCODE_2)
							setEntityType(PathFinder::ENT_PLAYER);
						if (event.key.keysym.scancode == SDL_SCANCODE_3)
							setEntityType(PathFinder::ENT_END);
						break;
					}
				default:
					break;
			}
		}
		const uint8_t* kb_state = SDL_GetKeyboardState(nullptr);
		handleWidgetInputs();
	}
}

MainMenuBtn::MainMenuBtn():
	TextButton("Main Menu",
			5, WINDOW_H - 110,
			100, 50, 
			Font::openSansSmall){}

void MainMenuBtn::handleInputs(){
	if (isMouseOver() && isClicked()){
		App::getInstance()->SceneManager::switchScene("MAIN_MENU");
	}
}

ExitBtn::ExitBtn():
	TextButton("Exit",
			5, WINDOW_H - 55,
			100, 50,
			Font::openSansSmall,
			Color::RED){}

void ExitBtn::handleInputs() {
	if (isMouseOver() && isClicked()){
		std::cout << "Exiting the game.\n";
		App::getInstance()->setRunning(false);
	}
}

