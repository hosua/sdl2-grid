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

class App {
	public:
		// Singleton. This will ensure that App can only be instantiated once
		static App* getInstance(){ 
			if (_instance == nullptr)
				_instance = new App();
			return _instance;
		}

		void addScene(std::unique_ptr<Scene> scene);
		bool renderScenes();

		void handleInputs();
		bool initSDL();

		bool isRunning() const;
		void setRunning(bool flag){ _running = flag; }

		SDL_Renderer* & getRenderer(){ return _renderer; }

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
		SDL_Window* _window = nullptr;
		SDL_Surface* _surface = nullptr;
		SDL_Renderer* _renderer = nullptr;
		SDL_Event _event;
		SceneManager _scene_manager;
};
