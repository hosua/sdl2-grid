#include <algorithm>
#include <memory>
#include <iostream>

#include "app.hh"
#include "defs.hh"

App* App::_instance = nullptr;

App::App():
	_scene_manager(_renderer) { 
	_running = initSDL() && Font::init();
}

App::~App(){
	SDL_DestroyRenderer(_renderer);
	_renderer = nullptr;
	SDL_FreeSurface(_surface);
	_surface = nullptr;
	SDL_DestroyWindow(_window);
	_window = nullptr;
	Font::close();
	SDL_Quit();
}


void App::addScene(std::unique_ptr<IScene> scene){
	_scene_manager.addScene(std::move(scene));
}

void App::mainLoop(){
	while (isRunning()){
		if (!renderScenes())
			setRunning(false);
		
		const SDL_Point& mouse_pos = GetMousePos();
		SDL_Event event;
		while (SDL_PollEvent(&event)){
			GetFrameEvents().push_back(event);
			switch(event.type){
				case SDL_KEYDOWN:
					{
						if (event.key.keysym.scancode == SDL_SCANCODE_M)
							fprintf(stdout, "Mouse position: (%i,%i)\n", mouse_pos.x, mouse_pos.y);
						if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
							setRunning(false);
						break;
					}
				default:
					break;
			}
		}

		_scene_manager.handleAllSceneInputs();

		GetFrameEvents().clear();
		SDL_Delay(17);
	}
}

bool App::renderScenes(){
	return _scene_manager.renderScenes();
}

bool App::isRunning() const { return _running; }

bool App::initSDL(){
	if (SDL_Init(SDL_INIT_EVERYTHING) > 0){
		std::cerr << "SDL_Init Error: " << SDL_GetError() << '\n';
		return false;
	} 


	_window = SDL_CreateWindow("Game Engine",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_W, WINDOW_H, 
			0);

	SDL_SetWindowBordered(_window, SDL_FALSE); // remove window border

	if (!_window){
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << '\n';
		return false;
	}

	_renderer = SDL_CreateRenderer(_window, -1, 0);
	if (!_renderer){
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << '\n';
		return false;
	}

	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND); // but will it blend?

	_surface = SDL_GetWindowSurface(_window);
	if (!_surface){
		std::cerr << "SDL_GetWindowSurface Error: " << SDL_GetError() << '\n';
		return false;
	}

	TTF_Init();

	return true;
}
