#include "test.hh"

#include <iostream>

#include "ui/all.hh"
#include "ui/spinner.hh"

int v_val = 0; 
float h_val = 0.0;

const int WORLD_X = 200, WORLD_Y = 200, WORLD_W = 400, WORLD_H = 400;

Test::Test(SDL_Renderer* &renderer, SceneManager& scene_mgr, bool& running):
	IScene("TEST_MENU", renderer),
	World(WORLD_X, WORLD_Y, WORLD_W, WORLD_H, running),
	_running(running) {

		// vertical spinner	
		std::unique_ptr<UI::Spinner<int>> v_spnr =
			std::make_unique<UI::Spinner<int>>(v_val,
					50, 50,
					26, 65,
					0, 100, 1,
					renderer,
					UI::ST_VERTICAL
					);
		addWidget(std::move(v_spnr));	

		// horizontal spinner
		std::unique_ptr<UI::Spinner<float>> h_spnr =
			std::make_unique<UI::Spinner<float>>(h_val,
					100, 50,
					65, 25,
					0., 1., .01,
					renderer,
					UI::ST_HORIZONTAL
					);
		addWidget(std::move(h_spnr));

		std::unique_ptr<MainMenuBtn> btn_main_menu = 
			std::make_unique<MainMenuBtn>(renderer, scene_mgr);
		addWidget(std::move(btn_main_menu));

		std::unique_ptr<ExitBtn> btn_exit = 
			std::make_unique<ExitBtn>(renderer, _running);
		addWidget(std::move(btn_exit));
	};

void Test::render(SDL_Renderer* &renderer) {
	World::draw(renderer);
	renderWidgets();
};

void Test::handleInputs(){
	const SDL_Point& mouse_pos = GetMousePos();
	static bool lmb_down = false, rmb_down = false;
	for (const SDL_Event& event : GetFrameEvents()){
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

MainMenuBtn::MainMenuBtn(SDL_Renderer* &renderer, SceneManager& scene_mgr):
	Button("Main Menu",
			5, WINDOW_H - 110,
			100, 50, 
			renderer,
			Font::openSansSmall),
	_scene_mgr(scene_mgr) {}

void MainMenuBtn::handleInputs(){
	if (isMouseOver() && isClicked()){
		_scene_mgr.switchScene("MAIN_MENU");
	}
}

ExitBtn::ExitBtn(SDL_Renderer* &renderer, bool& running):
	Button("Exit",
			5, WINDOW_H - 55,
			100, 50,
			renderer, 
			Font::openSansSmall,
			Color::RED),
	_running(running) {}

void ExitBtn::handleInputs() {
	if (isMouseOver() && isClicked()){
		std::cout << "Exiting the game.\n";
		_running= false;
	}
}

