#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <memory>

#include "app.hh"
#include "scenes/game.hh"
#include "scenes/test.hh"

int main(){
	srand(time(NULL));

	App* app = App::getInstance();

	if (!app->isRunning())
		return EXIT_FAILURE;

	std::unique_ptr<Game> game_scene = std::make_unique<Game>(app->getRenderer());
	app->addScene(std::move(game_scene));

	// std::unique_ptr<Test> test_scene = std::make_unique<Test>(app->getRenderer());
	// app->addScene(std::move(test_scene));
	
	app->mainLoop();

	return EXIT_SUCCESS;
}


std::vector<SDL_Event>& GetFrameEvents(){
	static std::vector<SDL_Event> event_list;
	return event_list;
}

const SDL_Point& GetMousePos(){
	static SDL_Point mouse_pos;
	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	return mouse_pos;
}
