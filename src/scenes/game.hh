#pragma once
#include "../scene.hh"
#include "../world.hh"

#include <functional>

class Game : public Scene {
public:
	Game(SDL_Renderer* &renderer);
	~Game() = default;

	bool render(SDL_Renderer* &renderer) override;
	void handleInputs(SDL_Point& mouse_pos) override;

	void drawWorld(SDL_Renderer* &renderer);
	
	// gets and stores the path from player -> goal in _path.
	bool getPath(std::function<std::vector<SDL_Point>(World& world, std::vector<SDL_Point> path)> helper); // return false if no path is found
	void renderPath(SDL_Renderer* &renderer); // renders _path (if one can be formed)
	// move player relative to current pos, returns true if player moved
	bool movePlayer(int dx, int dy); 

private:
	bool _end_game = false;
	bool _render_path_flag = false;
	World _world;
	std::vector<SDL_Point> _path;
};
