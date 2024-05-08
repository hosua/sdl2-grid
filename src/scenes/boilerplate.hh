#pragma once

#include "scene.hh"
#include "defs.hh"

#include "ui/all.hh"

/* Use this file to copy the boilerplate code to use for new scenes. Do
 * not modify this file directly. 
 */

class Example : public IScene {
public:
	~Example() = default;
	Example(SDL_Renderer* &renderer);
	bool render(SDL_Renderer* &renderer) override;
	void handleInputs() override;
private:
	bool _end_game = false;
};

