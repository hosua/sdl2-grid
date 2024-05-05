#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>

class Scene {
public:
	Scene(const std::string& key);
	~Scene() = default;

	const std::string& getKey() const; 
	// render returns false if the game should shutdown
	virtual bool render(SDL_Renderer* &renderer) = 0;
	virtual void handleInputs(SDL_Point& mouse_pos) = 0;
	
	bool isPaused() const;
	bool isRendering() const;

	void setPaused(bool flag);
	void setRendering(bool flag);

	void togglePaused();
	void toggleRendering();

private:
	const std::string _key;
	bool _is_rendering = true;
	bool _is_paused = false; // inputs paused
};
