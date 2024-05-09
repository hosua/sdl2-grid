#include "test.hh"

#include <iostream>

#include "ui/all.hh"
#include "ui/spinner.hh"

#include "app.hh"

int v_val = 0; 
float h_val = 0.0;

const int WORLD_X = 200, WORLD_Y = 200, WORLD_W = 400, WORLD_H = 400;

Test::Test():
	IScene("TEST_MENU"),
	World(WORLD_X, WORLD_Y, WORLD_W, WORLD_H){

		// vertical spinner	
		std::unique_ptr<UI::Spinner<int>> v_spnr =
			std::make_unique<UI::Spinner<int>>(v_val,
					50, 50,
					26, 65,
					0, 100, 1,
					UI::ST_VERTICAL
					);
		addWidget(std::move(v_spnr));	

		// horizontal spinner
		std::unique_ptr<UI::Spinner<float>> h_spnr =
			std::make_unique<UI::Spinner<float>>(h_val,
					100, 50,
					65, 25,
					0., 1., .01,
					UI::ST_HORIZONTAL
					);
		addWidget(std::move(h_spnr));

		std::unique_ptr<MainMenuBtn> btn_main_menu = 
			std::make_unique<MainMenuBtn>();
		addWidget(std::move(btn_main_menu));

		std::unique_ptr<ExitBtn> btn_exit = 
			std::make_unique<ExitBtn>();
		addWidget(std::move(btn_exit));
	};

void Test::render(){
	World::draw();
	renderWidgets();
};

void Test::handleInputs(){

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

MainMenuBtn::MainMenuBtn():
	Button("Main Menu",
			5, WINDOW_H - 110,
			100, 50, 
			Font::openSansSmall){}

void MainMenuBtn::handleInputs(){
	if (isMouseOver() && isClicked()){
		App::getInstance()->SceneManager::switchScene("MAIN_MENU");
	}
}

ExitBtn::ExitBtn():
	Button("Exit",
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

