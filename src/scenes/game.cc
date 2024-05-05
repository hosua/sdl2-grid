#include "game.hh"
#include "ui.hh"

#include <iostream>

class FirstBtn : public UI::Button {
	public:
		FirstBtn(SDL_Renderer* renderer): Button(
				"Click Me", 
				200, 200,
				180, 50,
				renderer){}

		void handleInput(SDL_Point mouse_pos, const uint8_t* kb_state, SDL_Event& event) override {
			_mouse_over = SDL_PointInRect(&mouse_pos, &_rect);
			if (event.type == SDL_MOUSEBUTTONDOWN &&
					event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Nice, you clicked da first button.\n";
			}
		}


};

Game::Game(SDL_Event& event, SDL_Renderer* &renderer): Scene("GAME"), 
	_world(WINDOW_H / BLOCK_H, WINDOW_W / BLOCK_W),
	_renderer(renderer),
	_event(event),
	_ui_manager(event)
{
	_ui_manager.addWidget(new FirstBtn(_renderer));	
};

bool Game::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	drawWorld(renderer);
	SDL_Point mouse_pos;

	const uint8_t* kb_state = SDL_GetKeyboardState(nullptr);
	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	_ui_manager.renderAndHandleWidgets(mouse_pos, kb_state, renderer);

	return true;
};

void Game::handleInputs(SDL_Point& mouse_pos){
	SDL_Point g = { mouse_pos.x / BLOCK_W, mouse_pos.y / BLOCK_H };

	static bool lmb_down = false, rmb_down = false;

	while (SDL_PollEvent(&_event)){
		switch(_event.type){
			case SDL_MOUSEBUTTONDOWN:
				{
					if (_event.button.button == SDL_BUTTON_LEFT)
						lmb_down = true;
					else if (_event.button.button == SDL_BUTTON_RIGHT)
						rmb_down = true;
					break;
				}
			case SDL_MOUSEBUTTONUP:
				{
					if (_event.button.button == SDL_BUTTON_LEFT)
						lmb_down = false;
					else if (_event.button.button == SDL_BUTTON_RIGHT)
						rmb_down = false;
				}
			case SDL_KEYDOWN:
				{
					// if (_event.key.keysym.scancode == SDL_SCANCODE_P)
					if (_event.key.keysym.scancode == SDL_SCANCODE_M)
						fprintf(stdout, "Mouse: (%i,%i)\n", mouse_pos.x, mouse_pos.y);
					break;
				}
			default:
				break;
		}
	}
	
	// handle continuous mouse down events
	if (lmb_down) _world.spawnWall(g.x, g.y);
	if (rmb_down) _world.deleteWall(g.x, g.y);
	SDL_PumpEvents();
	const uint8_t* kb_state = SDL_GetKeyboardState(nullptr);
	if (kb_state[SDL_SCANCODE_ESCAPE]){
		_end_game = true;
		return;
	}

	static uint32_t playerLastMoved = 0;
	
	if (playerLastMoved == 0){ // add some delay between movement events
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

