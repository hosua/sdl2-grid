#include "world.hh"

const int WORLD_W = WINDOW_W - LEFT_PANE_W;
const int WORLD_H = WINDOW_H;

World::World(bool& render_path_flag): _render_path_flag(render_path_flag) {
	_rows = WORLD_H / BLOCK_H;
	_cols = WORLD_W / BLOCK_W;

	_grid = std::vector<std::vector<EntType>>(_rows, std::vector<EntType>(_cols, ENT_NONE));

	// start is unnecessary, the player will just be the start
	// _start = { 0, 0 };
	// _grid[_start.y][_start.x] = ENT_START;

	_player = { 0, 0 };
	_grid[_player.y][_player.x] = ENT_PLAYER;

	_end = { _cols-1, _rows-1 };
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
			rect = { LEFT_PANE_W + (x * BLOCK_W), y * BLOCK_H, BLOCK_W, BLOCK_H };
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	
	// draw gridlines above the entities
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	// draw horizontal
	for (int y = 0; y < _rows * BLOCK_H; y += BLOCK_H)
		SDL_RenderDrawLine(renderer, LEFT_PANE_W, y, LEFT_PANE_W + (_cols * BLOCK_W), y);
	// draw vertical
	for (int x = LEFT_PANE_W; x <= LEFT_PANE_W + (_cols * BLOCK_W); x += BLOCK_W)
		SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_H);
}

void World::movePlayer(int nx, int ny){
	if (inBounds(nx, ny) &&
			_grid[ny][nx] == ENT_NONE){
		_grid[_player.y][_player.x] = ENT_NONE;
		_player.x = nx, _player.y = ny;
		_grid[ny][nx] = ENT_PLAYER;
		_render_path_flag = false;
	}
}

// clearly renders only the world grid portion of the screen
void World::renderClear(SDL_Renderer* renderer){
	const SDL_Color c = Color::BLACK;
	SDL_Rect world_rect = { LEFT_PANE_W, 0, BLOCK_W * _cols, BLOCK_H * _rows };
	// clear world with black color
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255); 
	SDL_RenderFillRect(renderer, &world_rect);
	// redraw the world
	draw(renderer);

}

void World::spawnWall(int x, int y){
	if (inBounds(x, y) && _grid[y][x] == ENT_NONE)
		_grid[y][x] = ENT_WALL;
}

void World::deleteWall(int x, int y){
	if (inBounds(x, y) && _grid[y][x] == ENT_WALL)
		_grid[y][x] = ENT_NONE;
}

bool World::inBounds(int x, int y) const {
	return (x >= 0 && x < _cols && y >= 0 && y < _rows);
}

SDL_Point World::getStartPos() const {
	return _start;
}

SDL_Point World::getEndPos() const {
	return _end;
}

SDL_Point World::getPlayerPos() const {
	return _player;
}
// gets the entity type at the specified position
EntType World::getPos(int x, int y) const {
	if (inBounds(x, y))
		return _grid[y][x];
	return ENT_NONE;
}

bool World::getRenderPathFlag() const {
	return _render_path_flag;
}

void World::setRenderPathFlag(bool flag){ 
	_render_path_flag = flag; 
}

const std::pair<int,int> World::getDimensions(){
	return std::make_pair(_rows, _cols);
}
