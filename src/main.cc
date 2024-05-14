#include <SDL2/SDL.h>

#include <chrono>
#include <iostream>
#include <random>
#include <memory>

#include "app.hh"
#include "scenes/game.hh"
#include "scenes/main_menu.hh"
#include "scenes/settings.hh"

Settings g_settings;

int main(){
	srand(time(NULL));

	if (!App::getInstance()->isRunning())
		return EXIT_FAILURE;

	std::unique_ptr<Scenes::MainMenu> main_menu_scene = 
		std::make_unique<Scenes::MainMenu>();

	App::getInstance()->addScene(std::move(main_menu_scene));
	App::getInstance()->addScene(std::make_unique<Scenes::Game>());
	App::getInstance()->addScene(std::make_unique<Scenes::Settings>());
	
	App::getInstance()->mainLoop();
	
	std::cout << "Process exited with no errors.\n";
	return EXIT_SUCCESS;
}



