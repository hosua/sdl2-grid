#include "world.hh"
#include "defs.hh"
World::World(uint16_t rows, uint16_t cols)
	: _rows(rows), _cols(cols),
		_grid(std::vector<std::vector<EntType>>(rows, std::vector<EntType>(cols, ENT_NONE))){
	
	_start = { 0, 0 };
	_end = { cols-1, rows-1 };
	_player = { 1, 1 };

	_grid[_start.y][_start.x] = ENT_START;
	_grid[_player.y][_player.x] = ENT_PLAYER;
	_grid[_end.y][_end.x] = ENT_END;
}

void World::draw(SDL_Renderer* renderer){
	const SDL_Color& c = Color::LIGHT_GREY;

	// draw entities in grid
	SDL_Rect rect;
	for (int y = 0; y < _rows; ++y){
		for (int x = 0; x < _cols; ++x){
			// draw the entity
			EntType ent_type = _grid[y][x];
			const SDL_Color& c = ENT_COLOR_MAP[ent_type];
			SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
			rect = { x * BLOCK_W, y * BLOCK_H, BLOCK_W, BLOCK_H };
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	// draw gridlines above the entities
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	// draw horizontal
	for (int y = 0; y < _rows * BLOCK_H; y += BLOCK_H)
		SDL_RenderDrawLine(renderer, 0, y, WINDOW_W, y);
	// draw vertical
	for (int x = 0; x < _cols * BLOCK_W; x += BLOCK_W)
		SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_H);
}

void World::movePlayer(int nx, int ny){
	if (inBounds(nx, ny) &&
			_grid[ny][nx] == ENT_NONE){
		_grid[_player.y][_player.x] = ENT_NONE;
		_player.x = nx, _player.y = ny;
		_grid[ny][nx] = ENT_PLAYER;
	}
}

void World::spawnWall(int x, int y){
	if (inBounds(x, y) && _grid[y][x] == ENT_NONE)
		_grid[y][x] = ENT_WALL;
}

void World::deleteWall(int x, int y){
	if (inBounds(x, y) && _grid[y][x] == ENT_WALL)
		_grid[y][x] = ENT_NONE;
}

bool World::inBounds(int x, int y){
	return (x >= 0 && x < _cols && y >= 0 && y < _rows);
}
SDL_Point World::getStartPos(){
	return _start;
}
SDL_Point World::getEndPos(){
	return _end;
}
SDL_Point World::getPlayerPos(){
	return _player;
}
