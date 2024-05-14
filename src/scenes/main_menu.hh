#pragma once

#include "scene.hh"
#include "scene_manager.hh"
#include "defs.hh"

#include "ui/all.hh"

namespace Scenes {

	class MainMenu : public IScene {
	public:
		~MainMenu() = default;
		MainMenu();
		void render() override;
		void handleInputs() override;
	private:
	};

}

namespace MainMenuWidgets {
	class StartBtn : public UI::TextButton {
	public:
		~StartBtn() = default;
		StartBtn();
		void handleInputs() override;
	private:
	};

	
	class SettingsBtn : public UI::TextButton {
		public:
			SettingsBtn();

			void handleInputs() override;
		private:
	};

	class ExitBtn : public UI::TextButton {
		public:
			ExitBtn();
			void handleInputs() override;
		private:
	};
}
