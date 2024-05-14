#pragma once
#include <functional>
#include <iostream>

#include "../scene.hh"
#include "../pathfinder/world.hh"
#include "ui/all.hh"

#include "scene_manager.hh"

using namespace PathFinder;

class Game : public IScene, public World {
	public:
		Game();
		~Game() = default;

		void render() override;
		void handleInputs() override;

		// gets and stores the path from player -> goal in _path.
		bool getPath(std::function<std::vector<SDL_Point>(World& world, std::vector<SDL_Point> path)> helper); // return false if no path is found
		void renderPath(); // renders _path (if one can be formed)

		// sets the entity type that the player will emplace when clicking on the world
		void setEntityType(EntType entity_type){ _entity_type = entity_type; }

		// renders a rect behind the button of which entity type is currently selected
		void renderSelectedEntityType();

	
	private:
		std::vector<SDL_Point> _path;
		EntType _entity_type = ENT_WALL; // the current type of entity to spawn/move when clicking
		int _search_speed = 5; // the speed of the pathfinding search
};


namespace GameWidgets {
	class DFSBtn : public UI::Button {
		public:
			~DFSBtn() = default;
			DFSBtn(World& world,
					std::vector<SDL_Point>& path,
					const int& search_speed):
				Button("DFS",
						5, 5,
						130, 50),
				_world(world),
				_path(path),
				_search_speed(search_speed) {}

			void handleInputs() override;
		private:
			World& _world;
			std::vector<SDL_Point>& _path;
			const int& _search_speed;
	};

	class BFSBtn : public UI::Button {
		public:
			~BFSBtn() = default;
			BFSBtn(World& world,
					std::vector<SDL_Point>& path,
					const int& search_speed):
				Button("BFS", 
						5, 60, 
						130, 50),
				_world(world),
				_path(path),
				_search_speed(search_speed) {}

			void handleInputs() override;

		private:
			World& _world;
			std::vector<SDL_Point>& _path;
			const int& _search_speed;
	};

	class AStarBtn : public UI::Button {
		public:
			AStarBtn(World& world,
					std::vector<SDL_Point>& path,
					const int& search_speed):
				Button("A*", 
						5, 115, 
						130, 50),
				_world(world),
				_path(path),
				_search_speed(search_speed) {}

			void handleInputs() override;
		private:
			World& _world;
			std::vector<SDL_Point>& _path;
			const int& _search_speed;
	};


	class SelectEntWallBtn : public UI::Button {
		public:
			SelectEntWallBtn(EntType &ent_type):
				Button("",
						5, 170,
						40, 40,
						Font::openSansSmall,
						Color::GREY),
				_ent_type(ent_type) {}
			void handleInputs() override;
		private:
			EntType& _ent_type;
	};

	class SelectEntPlayerBtn : public UI::Button {
		public:
			SelectEntPlayerBtn(EntType &ent_type):
				Button("",
						50, 170,
						40, 40,
						Font::openSansSmall,
						Color::BLUE),
				_ent_type(ent_type) {}
			void handleInputs() override;
		private:
			EntType& _ent_type;
	};

	class SelectEntEndBtn : public UI::Button {
		public:
			SelectEntEndBtn(EntType &ent_type):
				Button("",
						95, 170,
						40, 40,
						Font::openSansSmall,
						Color::RED),
				_ent_type(ent_type) {}
			void handleInputs() override;
		private:
			EntType& _ent_type;
	};

	class MainMenuBtn : public UI::Button {
		public:
			MainMenuBtn():
				Button("Main Menu",
						5, WINDOW_H - 110,
						130, 50,
						Font::openSansSmall){}
			void handleInputs() override;
		private:
	};

	class ExitBtn : public UI::Button {
		public:
			ExitBtn():
				Button("Exit",
						5, WINDOW_H - 55,
						130, 50,
						Font::openSansSmall,
						Color::RED){}
			void handleInputs() override;
		private:
	};
}
