#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>

#include "ui.hh"

class Scene {
public:
	Scene(const std::string& key, SDL_Renderer* &renderer);
	~Scene() = default;

	const std::string& getKey() const; 
	// render returns false if the game should shutdown
	virtual bool render(SDL_Renderer* &renderer) = 0;
	virtual void handleInputs(SDL_Point& mouse_pos) = 0;
	
	bool addWidget(std::unique_ptr<UI::IWidget> widget); 
	bool removeWidget(uint32_t id);
	
	void renderWidgets();
	void handleWidgetInputs(SDL_Event event);
	
private:
	const std::string& _key;
	UI::WidgetManager _widget_mgr;
	SDL_Renderer* &_renderer;
};
