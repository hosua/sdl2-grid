#include "game.hh"
#include "ui.hh"

#include <iostream>

class DFSBtn : public UI::Button {
	public:
		~DFSBtn() = default;
		DFSBtn(SDL_Renderer* &renderer): 
			Button("DFS",
					5, 5,
					130, 50,
					renderer){}

		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && 
				event.type == SDL_MOUSEBUTTONDOWN &&
				event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Finding path with DFS!\n";
			}
		}
};

class BFSBtn : public UI::Button {
public:
	BFSBtn(SDL_Renderer* &renderer):
		Button("BFS", 
				5, 60, 
				130, 50,
				renderer){}
		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && 
				event.type == SDL_MOUSEBUTTONDOWN &&
				event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Finding path with BFS!\n";
				// do BFS logic hurr
			}
		}
};

class AStarBtn : public UI::Button {
public:
	AStarBtn(SDL_Renderer* &renderer):
		Button("A*", 
				5, 115, 
				130, 50,
				renderer){}
	void handleInputs(SDL_Event event) override {
		if (isMouseOver() && 
			event.type == SDL_MOUSEBUTTONDOWN &&
			event.button.button == SDL_BUTTON_LEFT){
			std::cout << "Finding path with A* search!\n";
			// do a* logic hurr
		}
	}
};

Game::Game(SDL_Renderer* &renderer): Scene("GAME", renderer){
		std::unique_ptr<DFSBtn> btn_dfs = std::unique_ptr<DFSBtn>(new DFSBtn(renderer));
		std::unique_ptr<BFSBtn> btn_bfs = std::unique_ptr<BFSBtn>(new BFSBtn(renderer));
		std::unique_ptr<AStarBtn> btn_astar = std::unique_ptr<AStarBtn>(new AStarBtn(renderer));

		addWidget(std::move(btn_dfs));
		addWidget(std::move(btn_bfs));
		addWidget(std::move(btn_astar));
	};

bool Game::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	drawWorld(renderer);
	renderWidgets();
	return true;
};

void Game::handleInputs(SDL_Point& mouse_pos){
	SDL_Point g = { (mouse_pos.x - LEFT_PANE_W) / BLOCK_W, mouse_pos.y / BLOCK_H };

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

