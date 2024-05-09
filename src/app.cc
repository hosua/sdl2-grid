#include <algorithm>
#include <chrono>
#include <memory>
#include <iostream>

#include "app.hh"
#include "defs.hh"
// Gets a global vector of SDL_Events, which are polled in App::mainLoop().
// This is necessary to handle events outside of while(SDL_PollEvents()).
std::vector<SDL_Event>& App::getFrameEvents(){
	static std::vector<SDL_Event> event_list;
	return event_list;
}

const SDL_Point& App::getMousePos(){
	static SDL_Point mouse_pos;
	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	return mouse_pos;
}

// Stops the current thread from running for float (ms) time. Converted to
// ns for higher resolution.
void App::delayHighRes(float ms){
	int ns = (ms * (float)1e6);
	auto start_time = std::chrono::steady_clock::now();
	while ((std::chrono::steady_clock::now() - start_time) < std::chrono::nanoseconds(ns))
		continue;
}


App* App::_instance = nullptr;

App::App():
	SceneManager(){
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

void App::mainLoop(){
	while (isRunning()){
		SceneManager::renderScenes();
		
		const SDL_Point& mouse_pos = getMousePos();
		SDL_Event event;
		while (SDL_PollEvent(&event)){
			getFrameEvents().push_back(event);
			switch(event.type){
				case SDL_KEYDOWN:
					{
						if (event.key.keysym.scancode == SDL_SCANCODE_M)
							fprintf(stdout, "Mouse position: (%i,%i)\n", mouse_pos.x, mouse_pos.y);
						if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
							std::cout << "ESC was pressed\n";
							setRunning(false);
						}
						break;
					}
				default:
					break;
			}
		}

		SceneManager::handleAllSceneInputs();

		getFrameEvents().clear();
		SDL_Delay(17);
	}
	printf("_running = %i\n", _running);
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
