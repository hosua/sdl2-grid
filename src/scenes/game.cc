#include <map>
#include <set>
#include <queue>
#include <utility>
#include <thread>
#include <future>

#include "game.hh"
#include "defs.hh"

void DFSBtn::handleInputs(SDL_Event event) {
	if (isMouseOver() && isClicked(event)){
		std::cout << "Finding path with DFS!\n";
		_search_markers.clear();
		_path.clear();
		auto future = std::async(std::launch::async,
				&PathFinder::dfs, std::ref(_world), _search_speed, std::ref(_search_markers)); 
		_render_path_flag = true;
		// _path = future.get();

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

void BFSBtn::handleInputs(SDL_Event event) {
	if (isMouseOver() && isClicked(event)){
		std::cout << "Finding path with BFS!\n";

		_path = PathFinder::bfs(_world, _search_speed, _renderer); 
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

void AStarBtn::handleInputs(SDL_Event event) {
	if (isMouseOver() && isClicked(event)){
		std::cout << "A* not yet implemented!\n";
		// do a* logic hurr
	}
}

void SelectEntWallBtn::handleInputs(SDL_Event event) {
	if (isMouseOver() && isClicked(event)){
		_ent_type = ENT_WALL;
	}
}

void SelectEntPlayerBtn::handleInputs(SDL_Event event) {
	if (isMouseOver() && isClicked(event)){
		_ent_type = ENT_PLAYER;
	}
}

void SelectEntEndBtn::handleInputs(SDL_Event event) {
	if (isMouseOver() && isClicked(event)){
		_ent_type = ENT_END;
	}
}

void ExitBtn::handleInputs(SDL_Event event) {
	if (isMouseOver() && isClicked(event)){
		std::cout << "Exiting the game.\n";
		_end_game = true;
	}
}	

Game::Game(SDL_Renderer* &renderer):
	IScene("GAME", renderer), _world(_render_path_flag) {

		std::unique_ptr<DFSBtn> btn_dfs = 
			std::make_unique<DFSBtn>(_world, _path, _search_markers, renderer, _render_path_flag, _search_speed);
		addWidget(std::move(btn_dfs));

		std::unique_ptr<BFSBtn> btn_bfs = 
			std::make_unique<BFSBtn>(_world, _path, renderer, _render_path_flag, _search_speed);
		addWidget(std::move(btn_bfs));

		// TODO: Removed while still unimplemented
		// std::unique_ptr<AStarBtn> btn_astar = 
		// 	std::make_unique<AStarBtn>(renderer);
		// addWidget(std::move(btn_astar));

		std::unique_ptr<SelectEntPlayerBtn> btn_player = 
			std::make_unique<SelectEntPlayerBtn>(renderer, _entity_type);
		addWidget(std::move(btn_player));

		std::unique_ptr<SelectEntWallBtn> btn_wall = 
			std::make_unique<SelectEntWallBtn>(renderer, _entity_type);
		addWidget(std::move(btn_wall));

		std::unique_ptr<SelectEntEndBtn> btn_end = 
			std::make_unique<SelectEntEndBtn>(renderer, _entity_type);
		addWidget(std::move(btn_end));

		std::unique_ptr<ExitBtn> btn_exit = 
			std::make_unique<ExitBtn>(renderer, _end_game);
		addWidget(std::move(btn_exit));

		// vertical spinner
		// std::unique_ptr<UI::Spinner<int>> test_spinner = 
		// 	std::make_unique<UI::Spinner<int>>(_search_speed, 
		// 				5, 400,		// 				40, 100, 
		// 				0, 10, 1
		// 				renderer,
		// 				UI::ST_VERTICAL
		// 			);

		// search speed label
		std::unique_ptr<UI::Text> search_speed_lbl =
			std::make_unique<UI::Text>("Search Speed",
					18, 380,
					renderer,
					Font::openSansSmall
					);

		addWidget(std::move(search_speed_lbl));

		// search speed spinner
		std::unique_ptr<UI::Spinner<int>> search_speed_spnr =
			std::make_unique<UI::Spinner<int>>(_search_speed,
					40, 400,
					65, 25,
					0, 10, 1,
					renderer,
					UI::ST_HORIZONTAL
					);
		addWidget(std::move(search_speed_spnr));


	};

// 1. Render the _search_markers if the vector length is greater than 0
// 2. Render the _path if the vector length is greater than 0
// 3. When the _path vector is finished rendering, the _search_markers
// should either be emptied or not rendered.
//
// When actions that can happen should we stop rendering the search_markers/path?
// 		a) If a wall is spawned while a search is being ran, it could
//			invalidate the path if the player creates a wall that was
//			searched.
//		b) If the player moves their position while a path is being
//			searched for, this will also create an invalid path, since the
//			player is no longer where they started.
//		
//		To get around these issues, we can simply just disable player
//		movement and entity placement while a search is running. Then
//		we can also add a button that stops any search and clears the
//		_path, as well as the _search_markers.
//
//		c) If a path cannot be found, the _path and _search_markers should be cleared

bool Game::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	drawWorld(renderer);
	
	// if (_world.getRenderPathFlag())
	// 	renderPath(renderer);
	std::cout << "Game Scene is rendering\n";
	std::cout << "_search_markers size: " << _search_markers.size() << '\n';
	if (_search_markers.size() > 0){
		// render the current search state
		SDL_Color c = Color::GREEN;
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 128);
		SDL_RenderFillRects(renderer, &_search_markers[0], _search_markers.size());
		// for (const SDL_Rect& r : _search_markers){
		// 	printf("(%i,%i,%i,%i), ", r.x, r.y, r.w, r.h);
		// }
		// printf("\n------------------------------------------\n");
		// SDL_RenderFillRect(renderer, &rect);
	}

	renderSelectedEntityType(renderer); // render selected rect around entity button
	renderWidgets();
	return true;
};

void Game::renderSelectedEntityType(SDL_Renderer* &renderer){
	SDL_Rect r;
	switch (_entity_type){
		case ENT_WALL: // 1st
			r = { 4, 169, 42, 42 };
			break;
		case ENT_PLAYER: // second
			r = { 49, 169, 42, 42 };
			break;
		case ENT_END: // third
			r = { 94, 169, 42, 42 };
			break;
		case ENT_NONE:
			r = { 0, 0, 0, 0 };
			std::cerr << "Warning: ENT_NONE is selected as the entity type, but this should not be possible.\n";
			break;
	}
	SDL_Color c = Color::Light::GREEN;
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawRect(renderer, &r);
}

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

	if (lmb_down) _world.spawnEntity(_entity_type, g.x, g.y);
	if (rmb_down) _world.deleteWall(g.x, g.y);

	SDL_PumpEvents();
	const uint8_t* kb_state = SDL_GetKeyboardState(nullptr);
	if (kb_state[SDL_SCANCODE_ESCAPE]){
		_end_game = true;
		return;
	} 

	if (kb_state[SDL_SCANCODE_1])
		setEntityType(ENT_WALL);

	if (kb_state[SDL_SCANCODE_2])
		setEntityType(ENT_PLAYER);

	if (kb_state[SDL_SCANCODE_3])
		setEntityType(ENT_END);

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

std::vector<SDL_Point> Game::getPath() const {
	return _path;
} 

// renders _path (if one can be formed)
void Game::renderPath(SDL_Renderer* &renderer){
	SDL_Color c = Color::Light::BLUE;
	// render transparent square
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 128);
	for (const SDL_Point pt : _path){
		SDL_Rect rect = {
			LEFT_PANE_W + pt.x * BLOCK_W, pt.y * BLOCK_H, BLOCK_W, BLOCK_H
		};
		SDL_RenderFillRect(renderer, &rect);
	}
}

void Game::setEntityType(EntType entity_type){
	_entity_type = entity_type;
}
