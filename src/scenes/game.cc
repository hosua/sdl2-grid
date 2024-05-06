#include "game.hh"
#include "defs.hh"
#include "ui.hh"
#include "pathfinders/dfs.hh"
#include "pathfinders/bfs.hh"

#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <utility>


class DFSBtn : public UI::Button {
	public:
		~DFSBtn() = default;
		DFSBtn(World& world,
				std::vector<SDL_Point>& path,
				SDL_Renderer* &renderer,
				bool& render_path_flag):
			Button("DFS",
					5, 5,
					130, 50,
					renderer),
			_world(world),
			_path(path),
			_renderer(renderer),
			_render_path_flag(render_path_flag){}

		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && 
					event.type == SDL_MOUSEBUTTONDOWN &&
					event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Finding path with DFS!\n";

				_path = dfs(_world, _renderer); 
				_render_path_flag = true;

				if (_path.size() == 0){
					std::cout << "No path found!\n";
				} else {
					std::cout << "Path: \n";
					for (const SDL_Point& pt : _path)
						printf("(%i,%i) -> ", pt.x, pt.y);
					std::cout << "\n";
				}
			}
		}
	private:
		World& _world;
		std::vector<SDL_Point>& _path;
		SDL_Renderer* &_renderer;
		bool& _render_path_flag;
};

class BFSBtn : public UI::Button {
	public:
		~BFSBtn() = default;
		BFSBtn(World& world,
				std::vector<SDL_Point>& path,
				SDL_Renderer* &renderer,
				bool& render_path_flag):
			Button("BFS", 
					5, 60, 
					130, 50,
					renderer),
			_world(world),
			_path(path),
			_renderer(renderer),
			_render_path_flag(render_path_flag) {}

		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && 
					event.type == SDL_MOUSEBUTTONDOWN &&
					event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Finding path with BFS!\n";

				_path = bfs(_world, _renderer); 
				_render_path_flag = true;

				if (_path.size() == 0){
					std::cout << "No path found!\n";
				} else {
					std::cout << "Path: \n";
					for (const SDL_Point& pt : _path)
						printf("(%i,%i) -> ", pt.x, pt.y);
					std::cout << "\n";
				}
			}
		}

	private:
		World& _world;
		std::vector<SDL_Point>& _path;
		SDL_Renderer* &_renderer;
		bool& _render_path_flag;
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

class ExitBtn : public UI::Button {
	public:
		ExitBtn(SDL_Renderer* &renderer, bool& end_game):
			Button("Exit",
					5, WINDOW_H - 55,
					130, 50,
					renderer, 
					Font::openSansSmall,
					Color::RED),
				_end_game(end_game) {}
		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && 
					event.type == SDL_MOUSEBUTTONDOWN &&
					event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Exiting the game.\n";
				_end_game = true;
			}
		}	
	private:
		bool& _end_game;
};

Game::Game(SDL_Renderer* &renderer):
	Scene("GAME", renderer), _world(_render_path_flag) {

		std::unique_ptr<DFSBtn> btn_dfs = std::unique_ptr<DFSBtn>(new DFSBtn(_world, _path, renderer, _render_path_flag));
		std::unique_ptr<BFSBtn> btn_bfs = std::unique_ptr<BFSBtn>(new BFSBtn(_world, _path, renderer, _render_path_flag));
		std::unique_ptr<AStarBtn> btn_astar = std::unique_ptr<AStarBtn>(new AStarBtn(renderer));
		std::unique_ptr<ExitBtn> btn_exit = std::unique_ptr<ExitBtn>(new ExitBtn(renderer, _end_game));

		addWidget(std::move(btn_dfs));
		addWidget(std::move(btn_bfs));
		addWidget(std::move(btn_astar));
		addWidget(std::move(btn_exit));
	};

bool Game::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	drawWorld(renderer);

	if (_world.getRenderPathFlag())
		renderPath(renderer);

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
		if (moved){ 
			playerLastMoved = PLAYER_MOVE_DELAY;
			_world.setRenderPathFlag(false);
		}
	} else {
		playerLastMoved--;
	}

}

void Game::drawWorld(SDL_Renderer* &renderer) {
	_world.draw(renderer);
};

// returns true if the player moved 
bool Game::movePlayer(int dx, int dy){
	SDL_Point pos = _world.getPlayerPos();
	_world.movePlayer(pos.x + dx, pos.y + dy); // internally handles boundary checks
	return (pos.x != _world.getPlayerPos().x ||
			pos.y != _world.getPlayerPos().y);
}

// gets and stores the path from player -> goal in _path.
// helper() is a function that uses the world to find the path. It does not
// modify world in any shape or form. helper() returns false when no path is found
// returns false if no path is found
bool Game::getPath(std::function<std::vector<SDL_Point>(World& world, std::vector<SDL_Point> path)> helper){ 
	_path = helper(_world, _path);
	return _path.size() > 0;
} 

// renders _path (if one can be formed)
void Game::renderPath(SDL_Renderer* &renderer){
	SDL_Color c = Color::LIGHT_BLUE;
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 128);
	for (const SDL_Point pt : _path){
		SDL_Rect rect = {
			LEFT_PANE_W + pt.x * BLOCK_W, pt.y * BLOCK_H, BLOCK_W, BLOCK_H
		};
		// render transparent square
		SDL_RenderFillRect(renderer, &rect);
	}
}
