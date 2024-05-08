#include "boilerplate.hh"

#include <iostream>

#include "ui/all.hh"

/* Use this file to copy the boilerplate code to use for new scenes. Do
 * not modify this file directly. 
 */

bool Example::render(SDL_Renderer* &renderer) {
	if (_end_game)
		return false;
	renderWidgets();
	return true;
};

void Example::handleInputs(){
	handleWidgetInputs();
}
Example::Example(SDL_Renderer* &renderer):
	IScene("EXAMPLE", renderer){
};


