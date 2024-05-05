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

private:
	const std::string& _key;
};
