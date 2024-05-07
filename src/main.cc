#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <memory>

#include "app.hh"
#include "defs.hh"
#include "ui.hh"

#include "scenes/game.hh"

int main(){
	srand(time(NULL));

	App* app = App::getInstance();
	// if isRunning is false at this point, something bad happened, abort!
	if (!app->isRunning())
		return EXIT_FAILURE;

	std::unique_ptr<Game> game_scene = std::make_unique<Game>(app->getRenderer());
	app->addScene(std::move(game_scene));

	while (app->isRunning()){
		if (!app->renderScenes())
			app->setRunning(false);
		SDL_Delay(17);
	}
	return EXIT_SUCCESS;
}

