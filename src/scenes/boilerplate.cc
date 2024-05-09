#include "boilerplate.hh"

#include <iostream>

#include "ui/all.hh"

/** Use this file to copy the boilerplate code to use for new scenes. Do
 * not modify this file directly. 
 *
 * This file is ommitted from the build process.
 */

Example::Example(SDL_Renderer* &renderer, bool& running):
	IScene("EXAMPLE", renderer),
	_running(running){}

bool Example::render(SDL_Renderer* &renderer) {
	renderWidgets();
	return _running;
}

void Example::handleInputs(){
	const SDL_Point& mouse_pos = GetMousePos();
	static bool lmb_down = false, rmb_down = false;
	for (const SDL_Event& event : GetFrameEvents()){
		switch(event.type){
			// Handle input events here
			default:
				break;
		}
	}


	handleWidgetInputs();
}
