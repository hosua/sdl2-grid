#include <SDL2/SDL.h>

#include <chrono>
#include <iostream>
#include <random>
#include <memory>

#include "app.hh"
#include "scenes/game.hh"
#include "scenes/main_menu.hh"
#include "scenes/test.hh"

int main(){
	srand(time(NULL));

	App* app = App::getInstance();

	if (!app->isRunning())
		return EXIT_FAILURE;

	std::unique_ptr<MainMenu> main_menu_scene = 
		std::make_unique<MainMenu>(app->getRenderer(), std::ref(*app), app->getRunningRef());
	app->addScene(std::move(main_menu_scene));

	std::unique_ptr<Game> game_scene = 
			std::make_unique<Game>(app->getRenderer(), std::ref(*app));
	app->addScene(std::move(game_scene));

	std::unique_ptr<Test> test_scene =
			std::make_unique<Test>(app->getRenderer(), std::ref(*app), app->getRunningRef());
	app->addScene(std::move(test_scene));
	
	app->mainLoop();
	
	std::cout << "Process exited with no errors.\n";
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


void DelayHighRes(float ms){
	int ns = (ms * (float)1e6);
	auto start_time = std::chrono::steady_clock::now();
	while ((std::chrono::steady_clock::now() - start_time) < std::chrono::nanoseconds(ns))
		continue;
}

