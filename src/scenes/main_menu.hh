#pragma once

#include "scene.hh"
#include "scene_manager.hh"
#include "defs.hh"

#include "ui/all.hh"

class MainMenu : public IScene {
public:
	~MainMenu() = default;
	MainMenu();
	void render() override;
	void handleInputs() override;
private:
};

namespace MainMenuWidgets {
	class StartBtn : public UI::Button {
	public:
		~StartBtn() = default;
		StartBtn();
		void handleInputs() override;
	private:
	};

	
	class TestBtn : public UI::Button {
		public:
			TestBtn();

			void handleInputs() override;
		private:
	};

	class ExitBtn : public UI::Button {
		public:
			ExitBtn();
			void handleInputs() override;
		private:
	};
}
