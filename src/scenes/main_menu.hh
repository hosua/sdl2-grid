#pragma once

#include "scene.hh"
#include "scene_manager.hh"
#include "defs.hh"

#include "ui/all.hh"

/* Use this file to copy the boilerplate code to use for new scenes. Do
 * not modify this file directly. 
 */

class MainMenu : public IScene {
public:
	~MainMenu() = default;
	MainMenu(SDL_Renderer* &renderer, SceneManager &scene_mgr, bool &running);
	bool render(SDL_Renderer* &renderer) override;
	void handleInputs() override;
private:
	bool _end_game = false;
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

	class ExitBtn : public UI::Button {
		public:
			ExitBtn(SDL_Renderer* &renderer, bool& running);
			void handleInputs() override;
		private:
			bool& _running;
	};
}
