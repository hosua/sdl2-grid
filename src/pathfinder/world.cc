#include "world.hh"

#include <cstdio>
#include <stack>
#include <set>
#include <unordered_map>

#include "defs.hh"

#include "app.hh"

static std::vector<SDL_Point> cardinal_4 = {{0, +1}, {+1, 0}, {-1, 0}, {0, -1}};
static std::vector<SDL_Point> cardinal_8 = {{0, +1}, {+1, 0}, {-1, 0}, {0, -1},
											{+1,+1}, {-1,+1}, {-1,-1}, {+1,-1}};

static bool in_bounds(int x, int y, const std::vector<std::vector<PathFinder::EntType>>& grid){
	return (x >= 0 && x < (int)grid[0].size() && y >= 0 && y < (int)grid.size());
}

namespace PathFinder {
	const std::unordered_map<int, float> SEARCH_SPEED_MAP = {
		{0,  18.0},
		{1,  16.0},
		{2,  14.0},
		{3,  12.0},
		{4,  10.0},
		{5,  8.0 },
		{6,  6.0 },
		{7,  4.0 },
		{8,  2.0 },
		{9,  1.0 },
		{10, 0.5 },
	};

	std::unordered_map<EntType, SDL_Color> ENT_COLOR_MAP = {
		{ ENT_NONE, Color::BLACK },
		{ ENT_END, Color::RED },
		{ ENT_WALL, Color::GREY },
		{ ENT_PLAYER, Color::BLUE },
	};

	World::World(int x, int y,
			int w, int h):
			_rect({x,y,w,h}){

		_rows = h / BLOCK_H;
		_cols = w / BLOCK_W;
		std::printf("(rows,cols) = (%i,%i)\n", _rows, _cols);

		_grid = std::vector<std::vector<EntType>>(_rows, std::vector<EntType>(_cols, ENT_NONE));

		_player = { 0, 0 };
		_grid[_player.y][_player.x] = ENT_PLAYER;

		_end = { _cols-1, _rows-1 };
		_grid[_end.y][_end.x] = ENT_END;
	}

	void World::draw(){
		const SDL_Color& c = Color::Light::GREY;
		SDL_Renderer* renderer = App::getInstance()->getRenderer();
		// draw entities in grid
		SDL_Rect rect;
		for (int y = 0; y < _rows; ++y){
			for (int x = 0; x < _cols; ++x){
				// draw the entity
				EntType ent_type = _grid[y][x];
				const SDL_Color& c = ENT_COLOR_MAP[ent_type];
				SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
				rect = { _rect.x + (x * BLOCK_W), _rect.y + (y * BLOCK_H), BLOCK_W, BLOCK_H };
				SDL_RenderFillRect(renderer, &rect);
			}
		}

		// draw gridlines above the entities
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		// draw horizontal
		for (int y = _rect.y; y < _rect.y + (_rows * BLOCK_H); y += BLOCK_H)
			SDL_RenderDrawLine(renderer, _rect.x,  y, _rect.x + (_cols * BLOCK_W), y);
		// draw vertical
		for (int x = _rect.x; x <= _rect.x + (_cols * BLOCK_W); x += BLOCK_W)
			SDL_RenderDrawLine(renderer, x, _rect.y, x, _rect.y + _rect.h);
	}

	bool World::movePlayer(int x, int y){
		if (inBounds(x, y) && _grid[y][x] == ENT_NONE){
			_grid[_player.y][_player.x] = ENT_NONE;
			_player.x = x, _player.y = y;
			_grid[y][x] = ENT_PLAYER;
			return true;
		}
		return false;
	}

	bool World::movePlayerRelative(int dx, int dy){
		int nx = dx + _player.x, ny = dy + _player.y;

		if (inBounds(nx, ny) && _grid[ny][nx] == ENT_NONE){
			_grid[_player.y][_player.x] = ENT_NONE;
			_player.x = nx, _player.y = ny;
			_grid[ny][nx] = ENT_PLAYER;
			return true;
		}
		return false;
	}

	// clears only the world grid portion of the screen
	void World::renderClear(){
		SDL_Renderer* renderer = App::getInstance()->getRenderer();
		const SDL_Color c = Color::BLACK;
		// clear world with black color
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255); 
		SDL_RenderFillRect(renderer, &_rect);
		// redraw the world
		draw();
	}

	void World::spawnEntity(const EntType entity_type, int x, int y){
		if (inBounds(x, y) && _grid[y][x] == ENT_NONE){
			switch(entity_type){
				case ENT_WALL:
					_grid[y][x] = entity_type;
					break;
				case ENT_PLAYER: 
					{
						SDL_Point old_pos = getPlayerPos();
						std::swap(_grid[y][x], _grid[old_pos.y][old_pos.x]);
						_player = { x, y };
						break;
					}
				case ENT_END:
					{
						SDL_Point old_pos = getEndPos();
						std::swap(_grid[y][x], _grid[old_pos.y][old_pos.x]);
						_end = { x, y };
						break;
					}
				case ENT_NONE:
					break;
			}
		}
	}

	void World::deleteWall(int x, int y){
		if (inBounds(x, y) && _grid[y][x] == ENT_WALL)
			_grid[y][x] = ENT_NONE;
	}
	// gets the entity type at the specified position
	EntType World::getEntityAt(int x, int y) const {
		if (inBounds(x, y))
			return _grid[y][x];
		return ENT_NONE;
	}

	const std::pair<int,int> World::getDimensions(){
		return std::make_pair(_rows, _cols);
	}
	
	// returns true if the start and end nodes in the grid are connected
	static bool is_connected(std::vector<std::vector<EntType>> grid){
		SDL_Point start, end;

		for (int y = 0; y < (int)grid.size(); ++y){
			for (int x = 0; x < (int)grid[y].size(); ++x){
				if (grid[y][x] == ENT_PLAYER)
					start = { x, y };
				else if (grid[y][x] == ENT_END)
					end = { x, y };
			}
		}
		
		std::set<std::pair<int,int>> vis;
		std::stack<SDL_Point> s;
		s.push(start);

		while (!s.empty()){
			SDL_Point node = s.top();
			s.pop();
			int x = node.x, y = node.y;
			vis.insert(std::make_pair(x, y));
			// printf("[%i,%i] ->", x, y);
			if (x == end.x && y == end.y)
				return true;
			for (const auto& [mx, my] : cardinal_4){
				int nx = x + mx, ny = y + my;
				if (in_bounds(nx, ny, grid) &&
						grid[ny][nx] != ENT_WALL &&
						vis.find(std::make_pair(nx, ny)) == vis.end()){
					s.push({ nx, ny });
				}
			}
		}
		return false;
	}
	
	// randomize the world using algorithm A (some shit I made up)
	void randomize_world_a(World& world){
		const float a = 0.25f;
		const auto& [rows, cols] = world.getDimensions();

		std::vector<std::vector<EntType>> grid(rows, std::vector<EntType>(cols, ENT_NONE));

		// fill the entire grid with walls 
		for (int y = 0; y < rows; ++y)
			for (int x = 0; x < cols; ++x)
					grid[y][x] = ENT_WALL;

		// place start pos on the world
		int sx, sy;
		sx = App::getInstance()->getRandInt(0, cols-1);
		sy = App::getInstance()->getRandInt(0, rows-1);
		grid[sy][sx] = ENT_PLAYER;

		// place end pos on the world
		int ex, ey;
		do {
			ex = App::getInstance()->getRandInt(0, cols-1);
			ey = App::getInstance()->getRandInt(0, rows-1);
		}
		while ((sx == ex && sy == ey)); // redo if end is equal to start
		grid[ey][ex] = ENT_END;

		while (!is_connected(grid)){
			// randomly remove some walls
			for (int y = 0; y < rows; ++y)
				for (int x = 0; x < cols; ++x)
					if (grid[y][x] == ENT_WALL &&
							App::getInstance()->getRandFloat(0.f, 1.f) < a)
						grid[y][x] = ENT_NONE;
		}

		world.setGrid(grid);
	}

	void conways_game_of_life_iteration(std::vector<std::vector<EntType>>& grid){
		int rows = grid.size();
		int cols = grid[0].size();

		for (int y = 0; y < rows; ++y){
			for (int x = 0; x < cols; ++x){
				int neighbor_count = 0;
				for (const auto& [mx, my] : cardinal_8){
					int nx = x + mx, ny = y + my;
					if (in_bounds(nx, ny, grid) &&
							grid[ny][nx] == ENT_WALL){
						neighbor_count++;	
					}
				}

				if (grid[y][x] == ENT_WALL){
					if (neighbor_count < 0) {
						// Any live cell with fewer than two live neighbors dies
						grid[y][x] = ENT_NONE;
					}
					else if (neighbor_count == 2 || neighbor_count == 3){
						// Any live cell with two or three live neighbors lives
					} 
					else {
						// Any live cell with more than three live neighbors dies
						grid[y][x] = ENT_NONE;
					}
				} else if (grid[y][x] == ENT_NONE){
					// Any dead cell with exactly three live neighbors becomes alive
					if (neighbor_count == 3)
						grid[y][x] = ENT_WALL;

				}
			}
		}
	}

	// randomize the world using Conway's Game of Life algorithm
	void randomize_world_b(World& world){
		// probability that cell will initially be a wall
		const float a = 0.33f; 
		// the number of iterations to process for Conway's GOL.
		const int iters = App::getInstance()->getRandInt(150, 200);

		const auto& [rows, cols] = world.getDimensions();
		std::vector<std::vector<EntType>> grid(rows, std::vector<EntType>(cols, ENT_NONE));

		// place start pos on the world
		int sx, sy;
		sx = App::getInstance()->getRandInt(0, cols-1);
		sy = App::getInstance()->getRandInt(0, rows-1);
		grid[sy][sx] = ENT_PLAYER;

		// place end pos on the world
		int ex, ey;
		do {
			ex = App::getInstance()->getRandInt(0, cols-1);
			ey = App::getInstance()->getRandInt(0, rows-1);
		}
		while ((sx == ex && sy == ey)); // redo if end is equal to start
		grid[ey][ex] = ENT_END;
		
		// seed the intial grid 
		for (int y = 0; y < rows; ++y){
			for (int x = 0; x < cols; ++x){
				if (App::getInstance()->getRandFloat(0.f, 1.f) < a){
					if (grid[y][x] == ENT_NONE)
						grid[y][x] = ENT_WALL;
				}
			}
		}
		
		// do Conway's GOL iter times
		for (int i = 0; i < iters; ++i)
			conways_game_of_life_iteration(grid);
	
		// Continue Conway's GOL Algorithm until the graph is connected
		while (!is_connected(grid)){
			conways_game_of_life_iteration(grid);
		}
		
		world.setGrid(grid);
	}
}
