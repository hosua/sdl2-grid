#pragma once

#include <SDL2/SDL.h>
#include <initializer_list>
#include <memory>
#include <vector>

#include "scene.hh"
#include "color.hh"

class SceneManager {
public:
	SceneManager(SDL_Renderer* &renderer);	
	~SceneManager() = default;
	
	void addScene(std::unique_ptr<IScene> scene);
	bool removeScene(size_t index);
	bool removeScene(const std::string& key);
	void clearScenes(); /*!< Deletes all scenes from existence */
	
	void handleAllSceneInputs();

	void drawClear(const SDL_Color& color = Color::BLACK) const; // always call at the beginning of game loop iteration
	void drawPresent() const; // always call at the end of game loop iteration
	
	// disables all scenes (input and rendering) except for the scene being switched to
	bool switchScene(const std::string& key);
	// enables rendering and inputs management for the scene being launched but
	// does not modify the flags for the other scenes.
	bool launchScene(const std::string& key);

	/** \brief Renders all scenes that have been added with App::addScene().
	 *
	 */
	bool renderScenes();

private:
	std::vector<std::unique_ptr<IScene>> _scenes;
	SDL_Renderer* &_renderer;
};

