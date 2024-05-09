#pragma once

#include "scene.hh"
#include "scene_manager.hh"
#include "defs.hh"
#include "ui/all.hh"

#include "pathfinder/world.hh"

using namespace PathFinder;

class Test : public IScene, public PathFinder::World {
public:
	Test();
	~Test() = default;

	void render() override;
	void handleInputs() override;

	// sets the entity type that the player will emplace when clicking on the world
	void setEntityType(EntType entity_type){ _entity_type = entity_type; }

private:
	EntType _entity_type = ENT_WALL;
};

class MainMenuBtn: public UI::Button {
	public:
		MainMenuBtn();
		void handleInputs() override;
	private:
};

class ExitBtn : public UI::Button {
	public:
		ExitBtn();
		void handleInputs() override;
	private:
};
