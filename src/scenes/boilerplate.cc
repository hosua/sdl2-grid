#include "boilerplate.hh"

#include <iostream>

#include "ui/all.hh"

/* Use this file to copy the boilerplate code to use for new scenes. Do
 * not modify this file directly. 
 */

Example::Example(SDL_Renderer* &renderer):
	IScene("EXAMPLE", renderer){
};

bool Example::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	renderWidgets();
	return true;
};

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
