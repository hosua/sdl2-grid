#pragma once

#include "scene.hh"
#include "scene_manager.hh"
#include "defs.hh"

#include "ui/all.hh"

class MainMenu : public IScene {
public:
	~MainMenu() = default;
	MainMenu(SDL_Renderer* &renderer, SceneManager &scene_mgr, bool &running);
	void render(SDL_Renderer* &renderer) override;
	void handleInputs() override;
private:
	bool _running;
};

namespace MainMenuWidgets {
	class StartBtn : public UI::Button {
	public:
		~StartBtn() = default;
		StartBtn(SDL_Renderer* &renderer, SceneManager &scene_manager);
		void handleInputs() override;
	private:
		SceneManager& _scene_mgr;
	};

	
	class TestBtn : public UI::Button {
		public:
			TestBtn(SDL_Renderer* &renderer, SceneManager& scene_mgr);

			void handleInputs() override;
		private:
			SceneManager& _scene_mgr;
	};

	class ExitBtn : public UI::Button {
		public:
			ExitBtn(SDL_Renderer* &renderer);
			void handleInputs() override;
		private:
	};
}
