#pragma once

#include "scene.hh"
#include "scene_manager.hh"
#include "defs.hh"
#include "ui/all.hh"

#include "pathfinder/world.hh"

using namespace PathFinder;

class Test : public IScene, public PathFinder::World {
public:
	Test(SDL_Renderer* &renderer, SceneManager& scene_mgr, bool& running);
	~Test() = default;
	Test(SDL_Renderer* &renderer);

	void render(SDL_Renderer* &renderer) override;

	void handleInputs() override;

	// sets the entity type that the player will emplace when clicking on the world
	void setEntityType(EntType entity_type){ _entity_type = entity_type; }

private:
	bool& _running;
	EntType _entity_type = ENT_WALL;
};

class MainMenuBtn: public UI::Button {
	public:
		MainMenuBtn(SDL_Renderer* &renderer, SceneManager& scene_mgr);
		void handleInputs() override;
	private:
		SceneManager& _scene_mgr;
};

class ExitBtn : public UI::Button {
	public:
		ExitBtn(SDL_Renderer* &renderer, bool& running);
		void handleInputs() override;
	private:
		bool& _running;
};
