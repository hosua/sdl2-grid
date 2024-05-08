#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>

#include "ui/widget_manager.hh"
#include "ui/widget.hh"

class IScene {
public:
	IScene(const std::string& key, SDL_Renderer* &renderer);
	~IScene() = default;

	const std::string& getKey() const; 
	// render returns false if the game should shutdown
	virtual bool render(SDL_Renderer* &renderer) = 0;
	virtual void handleInputs() = 0;
	
	bool addWidget(std::unique_ptr<UI::IWidget> widget); 
	bool removeWidget(uint32_t id);
	
	void renderWidgets();
	void handleWidgetInputs();
	
private:
	const std::string& _key;
	UI::WidgetManager _widget_mgr;
	SDL_Renderer* &_renderer;
};
