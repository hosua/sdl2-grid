#pragma once
#include <functional>
#include <iostream>

#include "../scene.hh"
#include "../world.hh"
#include "ui/all.hh"

#include "pathfinders/dfs.hh"
#include "pathfinders/bfs.hh"

class Game : public IScene {
public:
	Game(SDL_Renderer* &renderer, bool& running);
	~Game() = default;

	bool render(SDL_Renderer* &renderer) override;
	void handleInputs() override;

	void drawWorld(SDL_Renderer* &renderer);
	
	// gets and stores the path from player -> goal in _path.
	bool getPath(std::function<std::vector<SDL_Point>(World& world, std::vector<SDL_Point> path)> helper); // return false if no path is found
	void renderPath(SDL_Renderer* &renderer); // renders _path (if one can be formed)
	// move player relative to current pos, returns true if player moved
	bool movePlayer(int dx, int dy); 

	void setEntityType(EntType entity_type); // sets the entity type that the player will emplace when clicking on the world
	void renderSelectedEntityType(SDL_Renderer* &renderer); // renders a rect behind the button of which entity type is currently selected

private:
	bool& _running; 
	bool _render_path_flag = false;
	World _world;
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
					SDL_Renderer* &renderer,
					bool& render_path_flag,
					const int& search_speed):
				Button("DFS",
						5, 5,
						130, 50,
						renderer),
				_world(world),
				_path(path),
				_renderer(renderer),
				_render_path_flag(render_path_flag),
				_search_speed(search_speed) {}

			void handleInputs() override;
		private:
			World& _world;
			std::vector<SDL_Point>& _path;
			SDL_Renderer* &_renderer;
			bool& _render_path_flag;
			const int& _search_speed;
	};

	class BFSBtn : public UI::Button {
		public:
			~BFSBtn() = default;
			BFSBtn(World& world,
					std::vector<SDL_Point>& path,
					SDL_Renderer* &renderer,
					bool& render_path_flag,
					const int& search_speed):
				Button("BFS", 
						5, 60, 
						130, 50,
						renderer),
				_world(world),
				_path(path),
				_renderer(renderer),
				_render_path_flag(render_path_flag) ,
				_search_speed(search_speed) {}

			void handleInputs() override;

		private:
			World& _world;
			std::vector<SDL_Point>& _path;
			SDL_Renderer* &_renderer;
			bool& _render_path_flag;
			const int& _search_speed;
	};

	class AStarBtn : public UI::Button {
		public:
			AStarBtn(SDL_Renderer* &renderer):
				Button("A*", 
						5, 115, 
						130, 50,
						renderer){}
			void handleInputs() override;
	};


	class SelectEntWallBtn : public UI::Button {
		public:
			SelectEntWallBtn(SDL_Renderer* &renderer, EntType &ent_type):
			Button("",
					5, 170,
					40, 40,
					renderer,
					Font::openSansSmall,
					Color::GREY),
			_ent_type(ent_type) {}
			void handleInputs() override;
		private:
			EntType& _ent_type;
	};

	class SelectEntPlayerBtn : public UI::Button {
		public:
			SelectEntPlayerBtn(SDL_Renderer* &renderer, EntType &ent_type):
			Button("",
					50, 170,
					40, 40,
					renderer,
					Font::openSansSmall,
					Color::BLUE),
			_ent_type(ent_type) {}
			void handleInputs() override;
		private:
			EntType& _ent_type;
	};

	class SelectEntEndBtn : public UI::Button {
		public:
			SelectEntEndBtn(SDL_Renderer* &renderer, EntType &ent_type):
			Button("",
					95, 170,
					40, 40,
					renderer,
					Font::openSansSmall,
					Color::RED),
			_ent_type(ent_type) {}
			void handleInputs() override;
		private:
			EntType& _ent_type;
	};

	class ExitBtn : public UI::Button {
		public:
			ExitBtn(SDL_Renderer* &renderer, bool& running):
				Button("Exit",
						5, WINDOW_H - 55,
						130, 50,
						renderer, 
						Font::openSansSmall,
						Color::RED),
					_running(running) {}
			void handleInputs() override;
		private:
			bool& _running;
	};
}
