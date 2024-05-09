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

	if (!App::getInstance()->isRunning())
		return EXIT_FAILURE;

	std::unique_ptr<MainMenu> main_menu_scene = 
		std::make_unique<MainMenu>();
	App::getInstance()->addScene(std::move(main_menu_scene));

	std::unique_ptr<Game> game_scene = 
			std::make_unique<Game>();
	App::getInstance()->addScene(std::move(game_scene));

	std::unique_ptr<Test> test_scene =
			std::make_unique<Test>();
	App::getInstance()->addScene(std::move(test_scene));
	
	App::getInstance()->mainLoop();
	
	std::cout << "Process exited with no errors.\n";
	return EXIT_SUCCESS;
}



