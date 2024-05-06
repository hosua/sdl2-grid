#include "game.hh"
#include "defs.hh"
#include "ui.hh"

#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <utility>

static std::vector<SDL_Point> s_moves = {{0, +1}, {+1, 0}, {-1, 0}, {0, -1}};
static std::vector<SDL_Point> dfs(World& world, SDL_Renderer* &renderer);
static std::vector<SDL_Point> bfs(World& world, SDL_Renderer* &renderer);

// TODO: Improve this so that path finders will animate the pathfinding process
static std::vector<SDL_Point> dfs(World& world, SDL_Renderer* &renderer){
	std::vector<SDL_Point> path;
	std::set<std::pair<int,int>> vis;

	std::function<void(SDL_Point, World&, std::vector<SDL_Point>, std::vector<SDL_Point>&,
			std::set<std::pair<int,int>>&)> dfs_helper;

	SDL_Point goal = world.getEndPos();
	
	// set render color for path search marking
	SDL_Color c = Color::GREEN;
	SDL_Color c_bg = Color::BLACK;
	std::vector<SDL_Rect> search_markers; // store the rect of each node visited here
	

	dfs_helper = [&](SDL_Point pos, World& world, 
			std::vector<SDL_Point> curr_path, std::vector<SDL_Point>& end_path, std::set<std::pair<int,int>>& vis){
		curr_path.push_back({pos.x, pos.y}); // add to path
		vis.insert(std::make_pair(pos.x, pos.y)); // mark as visited
		
		const auto& [w_rows, w_cols] = world.getDimensions();
		SDL_Rect rect = { LEFT_PANE_W + pos.x * BLOCK_W, pos.y * BLOCK_H, BLOCK_W, BLOCK_H };
		search_markers.push_back(rect);
		
		const SDL_Rect* rects = &search_markers[0];
		
		SDL_Rect world_rect = { LEFT_PANE_W, 0, BLOCK_W * w_cols, BLOCK_H * w_rows };
		// clear world with black color
		SDL_SetRenderDrawColor(renderer, c_bg.r, c_bg.g, c_bg.b, 255); 
		SDL_RenderFillRect(renderer, &world_rect);
		// redraw the world
		world.draw(renderer);
		
		// render the current search
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 128);
		SDL_RenderFillRects(renderer, rects, search_markers.size());
		SDL_Delay(10); // add some delay to the animation
		SDL_RenderPresent(renderer);

		if (pos.x == goal.x && pos.y == goal.y){
			end_path = curr_path;
			// remove starter node in path (player is already on here)
			if (end_path.size() > 0)
				end_path.erase(end_path.begin());
			return;	
		}
		for (const SDL_Point& moves : s_moves){
			int nx, ny;
			nx = pos.x + moves.x, ny = pos.y + moves.y;
			std::pair<int,int> pr = std::make_pair(nx, ny);
			if (world.inBounds(nx, ny) &&
					(world.getPos(nx, ny) == ENT_NONE || world.getPos(nx, ny) == ENT_END) 
					&& vis.find(pr) == vis.end()){
				dfs_helper({nx, ny}, world, curr_path, end_path, vis);
				// immediately end the search if we already found a path
				if (end_path.size() > 0)
					return;
			}
		}
	};

	SDL_Point start = world.getPlayerPos();
	std::vector<SDL_Point> temp_path;

	
	// find the path
	dfs_helper(start, world, temp_path, path, vis);

	// animate the path we formed
	SDL_Color c_finish = Color::LIGHT_GREEN;

	SDL_SetRenderDrawColor(renderer, c_finish.r, c_finish.g, c_finish.b, 128);

	for (auto itr = path.rbegin(); itr != path.rend(); ++itr){
		const SDL_Point pt = *itr;
		const SDL_Rect rect = { LEFT_PANE_W + pt.x * BLOCK_W, pt.y * BLOCK_H, BLOCK_W, BLOCK_H };
		SDL_RenderFillRect(renderer, &rect);
		SDL_Delay(10);
		SDL_RenderPresent(renderer);
	}


	return path;
}

static std::vector<SDL_Point> bfs(World& world, SDL_Renderer* &renderer){
	std::vector<SDL_Point> path;
	std::set<std::pair<int,int>> vis;
	std::map<std::pair<int,int>, SDL_Point> parent;

	std::function<void(SDL_Point, World&, std::vector<SDL_Point>, std::vector<SDL_Point>&,
			std::set<std::pair<int,int>>&)> bfs_helper;

	SDL_Point start = world.getPlayerPos();
	SDL_Point goal = world.getEndPos();
	
	std::queue<SDL_Point> q; // <curr_node, parent>
	q.push(start);
	parent[std::make_pair(start.x, start.y)] = {-1,-1};

	while (!q.empty()){
		size_t breadth = q.size();
		while (breadth--){
			SDL_Point pos = q.front();
			q.pop();

			if (pos.x == goal.x && pos.y == goal.y){
				// reconstruct the traversed path
				std::pair<int,int> crawl = std::make_pair(pos.x, pos.y);
				SDL_Point c = { crawl.first, crawl.second };
				path.push_back(c);
				while (crawl.first != start.x || crawl.second != start.y){
					c = { crawl.first, crawl.second };
					path.push_back(c);
					SDL_Point p = parent[crawl];
					crawl.first = p.x, crawl.second = p.y;
				}

				// remove starter node in path (player is already on here)
				if (path.size() > 0)
					path.erase(path.begin());
				std::reverse(path.begin(), path.end());
				return path;
			}

			for (const SDL_Point& moves : s_moves){
				SDL_Point n = {pos.x + moves.x, pos.y + moves.y};
				std::pair<int,int> pr = std::make_pair(n.x, n.y);
				if (world.inBounds(n.x, n.y) &&
						(world.getPos(n.x, n.y) == ENT_NONE || world.getPos(n.x, n.y) == ENT_END) 
						&& vis.find(pr) == vis.end()){
					q.push(n);
					parent[std::make_pair(n.x, n.y)] = pos;
					vis.insert(std::make_pair(n.x, n.y)); // mark as visited
				}
			}
		}
	}


	return {}; // no path found
}

class DFSBtn : public UI::Button {
	public:
		~DFSBtn() = default;
		DFSBtn(World& world,
				std::vector<SDL_Point>& path,
				SDL_Renderer* &renderer): 
			Button("DFS",
					5, 5,
					130, 50,
					renderer),
			_world(world),
			_path(path),
			_renderer(renderer){}

		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && 
					event.type == SDL_MOUSEBUTTONDOWN &&
					event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Finding path with DFS!\n";

				_world.setPlayerMoveFlag(false);
				_path = dfs(_world, _renderer); 

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
};

class BFSBtn : public UI::Button {
	public:
		~BFSBtn() = default;
		BFSBtn(World& world,
				std::vector<SDL_Point>& path,
				SDL_Renderer* &renderer):
			Button("BFS", 
					5, 60, 
					130, 50,
					renderer),
			_world(world),
			_path(path),
			_renderer(renderer){}

		void handleInputs(SDL_Event event) override {
			if (isMouseOver() && 
					event.type == SDL_MOUSEBUTTONDOWN &&
					event.button.button == SDL_BUTTON_LEFT){
				std::cout << "Finding path with BFS!\n";

				_world.setPlayerMoveFlag(false);
				_path = bfs(_world, _renderer); 

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

	std::unique_ptr<DFSBtn> btn_dfs = std::unique_ptr<DFSBtn>(new DFSBtn(_world, _path, renderer));
	std::unique_ptr<BFSBtn> btn_bfs = std::unique_ptr<BFSBtn>(new BFSBtn(_world, _path, renderer));
	std::unique_ptr<AStarBtn> btn_astar = std::unique_ptr<AStarBtn>(new AStarBtn(renderer));

	addWidget(std::move(btn_dfs));
	addWidget(std::move(btn_bfs));
	addWidget(std::move(btn_astar));
};

bool Game::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	drawWorld(renderer);

	if (!_world.getPlayerMoveFlag())
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
			_world.setPlayerMoveFlag(true);
		}
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
