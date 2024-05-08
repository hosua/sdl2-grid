#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "defs.hh"
#include "color.hh"
#include "scene.hh"

class SceneManager {
public:
	SceneManager(SDL_Renderer* &renderer);	
	~SceneManager() = default;
	
	void addScene(std::unique_ptr<Scene> scene);
	bool removeScene(size_t index);
	bool removeScene(const std::string& key);

	void drawClear(const SDL_Color& color = Color::BLACK) const; // always call at the beginning of game loop iteration
	void drawPresent() const; // always call at the end of game loop iteration
	
	// renderScenes returns false if the game should shutdown
	bool renderScenes();

private:
	std::vector<std::unique_ptr<Scene>> _scenes;
	SDL_Renderer* &_renderer;
};

