#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>

#include "ui/widget_manager.hh"
#include "ui/widget.hh"

class IScene : public UI::WidgetManager {
public:
	IScene(const std::string key,
			bool is_handling_inputs=false, bool is_rendering=false);
	~IScene() = default;

	const std::string& getKey() const { return _key; }
	// render returns false if the game should shutdown
	virtual void render() = 0;
	virtual void handleInputs() = 0;
	// set all data in scene to original state (does nothing if not overriden)
	virtual void restart(){ return; };

	bool isHandlingInputs() const { return _is_handling_inputs; }
	void setHandlingInputs(bool flag){ _is_handling_inputs = flag; }

	bool isRendering() const { return _is_rendering; }
	void setRendering(bool flag){ _is_rendering = flag; }
	
private:
	bool _is_handling_inputs = false, _is_rendering = false;
	const std::string _key;
};
