#pragma once

#include "scene.hh"
#include "ui/all.hh"

/** Use this file to copy the boilerplate code to use for new scenes. Do
 * not modify this file directly. 
 *
 * This file is ommitted from the build process.
 */

class Example : public IScene {
public:
	~Example() = default;
	Example();
	void render() override;
	void handleInputs() override;
private:
};

