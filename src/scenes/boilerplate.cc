#include "boilerplate.hh"

#include <iostream>

#include "ui/all.hh"
#include "app.hh"

App* app = App::getInstance();

/** Use this file to copy the boilerplate code to use for new scenes. Do
 * not modify this file directly. 
 *
 * This file is ommitted from the build process.
 */

Example::Example():
	IScene("EXAMPLE"){}

void Example::render() {
	renderWidgets();
}

void Example::handleInputs(){
	const SDL_Point& mouse_pos = app->getMousePos();
	static bool lmb_down = false, rmb_down = false;
	for (const SDL_Event& event : app->getFrameEvents()){
		switch(event.type){
			// Handle input events here
			default:
				break;
		}
	}


	handleWidgetInputs();
}
