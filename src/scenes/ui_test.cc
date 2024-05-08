#include "ui_test.hh"

#include <iostream>

#include "ui/all.hh"
#include "ui/spinner.hh"

int v_val = 0;
float h_val = 0.0;

class ExitBtn : public UI::Button {
	public:
		ExitBtn(SDL_Renderer* &renderer, bool& end_game):
			Button("X",
					WINDOW_W - 55, 5,
					50, 50,
					renderer, 
					Font::openSansSmall,
					Color::RED),
				_end_game(end_game){}

		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && isClicked(event)){
				std::cout << "Exiting the game.\n";
				_end_game = true;
			}
		}


	private:
		bool& _end_game;
};

TestUI::TestUI(SDL_Renderer* &renderer):
	IScene("UI_TEST", renderer){

	// vertical spinner	
	std::unique_ptr<UI::Spinner<int>> v_spnr =
		std::make_unique<UI::Spinner<int>>(v_val,
					400, 400,
					26, 65,
					0, 100, 1,
					renderer,
					UI::ST_VERTICAL
				);
	addWidget(std::move(v_spnr));	

	// horizontal spinner
	std::unique_ptr<UI::Spinner<float>> h_spnr =
		std::make_unique<UI::Spinner<float>>(h_val,
					450, 400,
					65, 25,
					0., 1., .01,
					renderer,
					UI::ST_HORIZONTAL
				);
	addWidget(std::move(h_spnr));

	std::unique_ptr<ExitBtn> btn_exit = 
		std::make_unique<ExitBtn>(renderer, _end_game);
	addWidget(std::move(btn_exit));
};


bool TestUI::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	renderWidgets();
	return true;
};

void TestUI::handleInputs(SDL_Point& mouse_pos){
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		handleWidgetInputs(event);
	}
}

