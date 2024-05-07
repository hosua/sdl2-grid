#include "dfs.hh"

#include <set>
#include <utility>
#include <functional>
#include <vector>

static std::vector<SDL_Point> s_moves = {{0, +1}, {+1, 0}, {-1, 0}, {0, -1}};

std::vector<SDL_Point> PathFinder::dfs(World& world, SDL_Renderer* &renderer){
	std::vector<SDL_Point> path;
	std::set<std::pair<int,int>> vis;

	std::function<void(SDL_Point, World&, std::vector<SDL_Point>, std::vector<SDL_Point>&,
			std::set<std::pair<int,int>>&)> dfs_helper;

	SDL_Point goal = world.getEndPos();

	world.renderClear(renderer);
	world.draw(renderer);

	// set render color for path search marking
	SDL_Color c = Color::GREEN;
	std::vector<SDL_Rect> search_markers; // store the rect of each node visited here
	dfs_helper = [&](SDL_Point pos, World& world,
			std::vector<SDL_Point> curr_path, std::vector<SDL_Point>& end_path, 
			std::set<std::pair<int,int>>& vis){
		curr_path.push_back({pos.x, pos.y}); // add to path
		vis.insert(std::make_pair(pos.x, pos.y)); // mark as visited

		SDL_Rect rect = { LEFT_PANE_W + pos.x * BLOCK_W, pos.y * BLOCK_H, BLOCK_W, BLOCK_H };
		search_markers.push_back(rect);

		// render the current search
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 128);
		SDL_RenderFillRect(renderer, &rect);
		SDL_Delay(10); // add some delay to the animation
		SDL_RenderPresent(renderer);


		if (pos.x == goal.x && pos.y == goal.y){
			end_path = curr_path;
			// remove starter node in path (player is already on here)
			if (end_path.size() > 0)
				end_path.erase(end_path.begin());
			return;	
		}
		for (const SDL_Point& moves : s_moves){
			int nx, ny;
			nx = pos.x + moves.x, ny = pos.y + moves.y;
			std::pair<int,int> pr = std::make_pair(nx, ny);
			if (world.inBounds(nx, ny) &&
					(world.getPos(nx, ny) == ENT_NONE || world.getPos(nx, ny) == ENT_END) 
					&& vis.find(pr) == vis.end()){
				dfs_helper({nx, ny}, world, curr_path, end_path, vis);
				// immediately end the search if we already found a path
				if (end_path.size() > 0)
					return;
			}
		}
	};

	SDL_Point start = world.getPlayerPos();
	std::vector<SDL_Point> temp_path;


	// find the path
	dfs_helper(start, world, temp_path, path, vis);

	// animate the path we formed
	SDL_Color c_finish = Color::LIGHT_GREEN;
	SDL_SetRenderDrawColor(renderer, c_finish.r, c_finish.g, c_finish.b, 128);
	for (auto itr = path.rbegin(); itr != path.rend(); ++itr){
		const SDL_Point pt = *itr;
		const SDL_Rect rect = { LEFT_PANE_W + pt.x * BLOCK_W, pt.y * BLOCK_H, BLOCK_W, BLOCK_H };
		SDL_RenderFillRect(renderer, &rect);
		SDL_Delay(7);
		SDL_RenderPresent(renderer);
	}


	return path;
}
