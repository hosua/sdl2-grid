#pragma once
#include "../scene.hh"
#include "../world.hh"
#include "../defs.hh"
#include "../ui.hh"

#define ROW_ROWS 

class Game : public Scene {
public:
	Game(SDL_Event& event, SDL_Renderer* &renderer);
	~Game() = default;

	bool render(SDL_Renderer* &renderer) override;
	void handleInputs(SDL_Point& mouse_pos) override;

	void drawWorld(SDL_Renderer* &renderer);
	// void drawUI(SDL_Renderer* &renderer);
	
	// move player relative to current pos, returns true if player moved
	bool movePlayer(int dx, int dy); 

private:
	bool _end_game = false;
	World _world;
	SDL_Renderer* &_renderer;
	SDL_Event& _event;
	UI::Manager _ui_manager;
};
