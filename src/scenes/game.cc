#include "game.hh"
#include "ui.hh"

#include <iostream>

class FirstBtn : public UI::Button {
	public:
		~FirstBtn() = default;
		FirstBtn(SDL_Renderer* &renderer): 
			Button("First",
					200, 200,
					150, 50,
					renderer){}

		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && 
				event.type == SDL_MOUSEBUTTONDOWN &&
				event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Clicked first button!\n";
			}
		}
};

class SecondBtn : public UI::Button {
public:
	SecondBtn(SDL_Renderer* &renderer):
		Button("Second", 
				200, 405, 
				150, 50,
				renderer){}
		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && 
				event.type == SDL_MOUSEBUTTONDOWN &&
				event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Clicked second button!\n";
			}
		}
};

Game::Game(SDL_Renderer* &renderer): Scene("GAME", renderer), 
	_world(WINDOW_H / BLOCK_H, WINDOW_W / BLOCK_W){
		std::unique_ptr<FirstBtn> btn_1 = std::unique_ptr<FirstBtn>(new FirstBtn(renderer));
		std::unique_ptr<SecondBtn> btn_2 = std::unique_ptr<SecondBtn>(new SecondBtn(renderer));
		addWidget(std::move(btn_1));
		addWidget(std::move(btn_2));
	};

bool Game::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	drawWorld(renderer);
	renderWidgets();
	return true;
};

void Game::handleInputs(SDL_Point& mouse_pos){
	SDL_Point g = { mouse_pos.x / BLOCK_W, mouse_pos.y / BLOCK_H };

	static bool lmb_down = false, rmb_down = false;
	
	SDL_Event event;
	while (SDL_PollEvent(&event)){
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
					if (event.key.keysym.scancode == SDL_SCANCODE_M)
						fprintf(stdout, "Mouse: (%i,%i)\n", mouse_pos.x, mouse_pos.y);
					break;
				}
			default:
				break;
		}

		handleWidgetInputs(event);
	}

	if (lmb_down) _world.spawnWall(g.x, g.y);
	if (rmb_down) _world.deleteWall(g.x, g.y);
	SDL_PumpEvents();
	const uint8_t* kb_state = SDL_GetKeyboardState(nullptr);
	if (kb_state[SDL_SCANCODE_ESCAPE]){
		_end_game = true;
		return;
	}

	static uint32_t playerLastMoved = 0;

	if (playerLastMoved == 0){ // add some delay between movement g_events
		bool moved = false;
		if (kb_state[SDL_SCANCODE_UP] || kb_state[SDL_SCANCODE_W])
			moved |= movePlayer(0, -1);
		if (kb_state[SDL_SCANCODE_DOWN] || kb_state[SDL_SCANCODE_S])
			moved |= movePlayer(0, +1);
		if (kb_state[SDL_SCANCODE_LEFT] || kb_state[SDL_SCANCODE_A])
			moved |= movePlayer(-1, 0);
		if (kb_state[SDL_SCANCODE_RIGHT] || kb_state[SDL_SCANCODE_D])
			moved |= movePlayer(+1, 0);

		// if the player moved, reset the delay timer
		if (moved) playerLastMoved = PLAYER_MOVE_DELAY;
	} else {
		playerLastMoved--;
	}

}

void Game::drawWorld(SDL_Renderer* &renderer) {
	_world.draw(renderer);
};

bool Game::movePlayer(int dx, int dy){
	SDL_Point pos = _world.getPlayerPos();
	_world.movePlayer(pos.x + dx, pos.y + dy); // internally handles boundary checks
	// return true if new player position is not the same as original	
	return (pos.x != _world.getPlayerPos().x ||
			pos.y != _world.getPlayerPos().y);
}

