#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <memory>

#include "app.hh"
#include "scenes/game.hh"
#include "scenes/ui_test.hh"

int main(){
	srand(time(NULL));

	App* app = App::getInstance();
	// if isRunning is false at this point, something bad happened, abort!
	if (!app->isRunning())
		return EXIT_FAILURE;

	std::unique_ptr<Game> game_scene = std::make_unique<Game>(app->getRenderer());
	app->addScene(std::move(game_scene));

	// std::unique_ptr<Test> ui_test_scene = std::make_unique<Test>(app->getRenderer());
	// app->addScene(std::move(ui_test_scene));

	while (app->isRunning()){
		if (!app->renderScenes())
			app->setRunning(false);
		SDL_Delay(17);
	}
	return EXIT_SUCCESS;
}

