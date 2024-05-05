#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <memory>

#include "app.hh"
#include "defs.hh"
#include "ui.hh"

int main(){
	srand(time(NULL));

	App* app = App::getInstance();

	while (app->isRunning()){
		if (!app->renderScenes())
			app->setRunning(false);
		SDL_Delay(17);
	}
	return EXIT_SUCCESS;
}

