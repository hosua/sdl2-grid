#include <map>
#include <set>
#include <queue>
#include <utility>

#include "game.hh"
#include "../defs.hh"

void DFSBtn::handleInputs() {
	for (const SDL_Event& event : GetFrameEvents()){
		if (isMouseOver() && isClicked(event)){
			std::cout << "Finding path with DFS!\n";

			_path = PathFinder::dfs(_world, _search_speed, _renderer); 
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
}

void BFSBtn::handleInputs() {
	for (const SDL_Event& event : GetFrameEvents()){
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
}

void AStarBtn::handleInputs() {
	for (const SDL_Event& event : GetFrameEvents()){
		if (isMouseOver() && isClicked(event)){
			std::cout << "A* not yet implemented!\n";
			// do a* logic hurr
		}
	}
}

void SelectEntWallBtn::handleInputs() {
	for (const SDL_Event& event : GetFrameEvents()){
		if (isMouseOver() && isClicked(event)){
			_ent_type = ENT_WALL;
		}
	}
}

void SelectEntPlayerBtn::handleInputs() {
	for (const SDL_Event& event : GetFrameEvents()){
		if (isMouseOver() && isClicked(event)){
			_ent_type = ENT_PLAYER;
		}
	}
}

void SelectEntEndBtn::handleInputs() {
	for (const SDL_Event& event : GetFrameEvents()){
		if (isMouseOver() && isClicked(event)){
			_ent_type = ENT_END;
		}
	}
}

void ExitBtn::handleInputs() {
	for (const SDL_Event& event : GetFrameEvents()){
		if (isMouseOver() && isClicked(event)){
			std::cout << "Exiting the game.\n";
			_end_game = true;
		}
	}
}	

Game::Game(SDL_Renderer* &renderer):
	IScene("GAME", renderer), _world(_render_path_flag) {

		std::unique_ptr<DFSBtn> btn_dfs = 
			std::make_unique<DFSBtn>(_world, _path, renderer, _render_path_flag, _search_speed);
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

bool Game::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	drawWorld(renderer);

	if (_world.getRenderPathFlag())
		renderPath(renderer);

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

static uint32_t s_player_last_moved = 0;
void Game::handleInputs(){
	
	const SDL_Point& mouse_pos = GetMousePos();
	static bool lmb_down = false, rmb_down = false;
	for (const SDL_Event& event : GetFrameEvents()){
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

					if (event.key.keysym.scancode == SDL_SCANCODE_1)
						setEntityType(ENT_WALL);
					if (event.key.keysym.scancode == SDL_SCANCODE_2)
						setEntityType(ENT_PLAYER);
					if (event.key.keysym.scancode == SDL_SCANCODE_3)
						setEntityType(ENT_END);
					break;
				}
			default:
				break;
		}
	}
	handleWidgetInputs();

	SDL_Point g = { (mouse_pos.x - LEFT_PANE_W) / BLOCK_W, mouse_pos.y / BLOCK_H };
	if (lmb_down) _world.spawnEntity(_entity_type, g.x, g.y);
	if (rmb_down) _world.deleteWall(g.x, g.y);

	// SDL_PumpEvents();
	const uint8_t* kb_state = SDL_GetKeyboardState(nullptr);

	if (s_player_last_moved == 0){ // add some delay between movement events
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
			s_player_last_moved = PLAYER_MOVE_DELAY;
			_world.setRenderPathFlag(false);
		}
	} else {
		s_player_last_moved--;
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
