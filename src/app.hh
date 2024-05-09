#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "scene.hh"
#include "scene_manager.hh"

class App : public SceneManager {
	public:
		// Singleton. This will ensure that App can only be instantiated once
		static App* getInstance(){ 
			if (_instance == nullptr)
				_instance = new App();
			return _instance;
		}

		void mainLoop();

		void handleInputs();
		bool initSDL();

		bool isRunning() const;
		void setRunning(bool flag){ _running = flag; }
		bool& getRunningRef(){ return std::ref(_running); }

		SDL_Renderer* &getRenderer(){ return _renderer; }

		// move player relative to current pos, returns true if player moved
		bool movePlayer(int dx, int dy); 

		App(App* const) = delete;			 // Don't implement
		App& operator=(App const&) = delete; // Don't implement
		
	private:
		static App* _instance;
		App();
		~App();

		bool _running = false;
		SDL_Point _mouse_pos;
		// TODO: Make these shared_ptr. It would have made more sense but I'm kind of an idiot
		SDL_Window* _window = nullptr;
		SDL_Surface* _surface = nullptr;
		SDL_Renderer* _renderer = nullptr;
};
