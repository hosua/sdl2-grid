#include <functional>
#include <iostream>
#include <set>
#include <stack>
#include <utility>
#include <map>
#include <vector>

#include <thread>
#include <future>

#include "dfs.hh"

#include "../defs.hh"
#include "pathfinders/defs.hh"
#include "color.hh"

static std::vector<SDL_Point> s_moves = {{0, +1}, {+1, 0}, {-1, 0}, {0, -1}};

std::vector<SDL_Point> PathFinder::dfs(World& world, 
		const int& search_speed, 
		std::vector<SDL_Rect>& search_markers)
{

	search_markers.clear();
	std::vector<SDL_Point> path;
	std::set<std::pair<int,int>> vis;
	// TODO: vis seems redundant, we can just use the parent to check if we've
	// already visited a node.
	std::map<std::pair<int,int>, std::pair<int,int>> parent;

	//std::function<void(SDL_Point, World&, 
	//		std::vector<SDL_Point>, 
	//		std::vector<SDL_Point>&,
	//		std::set<std::pair<int,int>>&)> dfs_helper;


	SDL_Point start_pos = world.getPlayerPos();
	std::vector<SDL_Point> curr_path;
	// TODO: Would be wise to ensure the search_speed is a valid value but I'm too lazy rn
	const int search_delay = SEARCH_SPEED_MAP[search_speed];
	SDL_Point goal = world.getEndPos();

	std::stack<SDL_Point> stk;
	stk.push(start_pos);
	std::pair<int,int> start_pr = std::make_pair(start_pos.x, start_pos.y);
	parent[start_pr] = std::make_pair(-1, -1); // {-1,-1} just means there's no parent
	vis.insert(start_pr);

	while (!stk.empty()){
		SDL_Point pos = stk.top();
		printf("(%i,%i) -> ", pos.x, pos.y);
		stk.pop();
	 	SDL_Rect r = { LEFT_PANE_W + pos.x * BLOCK_W, pos.y * BLOCK_H, BLOCK_W, BLOCK_H };
		search_markers.push_back(r);

		PathFinder::delayHighRes(search_delay);

		if (pos.x == goal.x && pos.y == goal.y){
			std::cout << "DFS reached the goal, reconstructing the path\n";
			std::pair<int,int> crawl = std::make_pair(pos.x, pos.y);
			while (crawl != start_pr){
				path.push_back({ crawl.first, crawl.second });
				crawl = parent[crawl];
				// SDL_Delay(7);
			}
			break;
		}

		for (const SDL_Point& move : s_moves){
			int nx = pos.x + move.x;
			int ny = pos.y + move.y;	
			std::pair<int,int> pr = std::make_pair(nx, ny);
			if (world.inBounds(nx, ny) &&
					(world.getPos(nx, ny) == ENT_NONE || world.getPos(nx, ny) == ENT_END) &&
					vis.find(pr) == vis.end()){
				vis.insert(pr);
				parent[pr] = std::make_pair(pos.x, pos.y);
				stk.push({nx, ny});	

			}
		}
	}
	
	/* TODO: We should not be trying to multithread a recursive function. Rewrite this to be iterative */
	// world.renderClear(renderer);
	// world.draw(renderer);
	// SDL_Color c = Color::GREEN;
	// std::vector<SDL_Rect> search_markers; // store the rect of each node visited here
	
	// dfs_helper = [&](SDL_Point pos, World& world,
	// 		std::vector<SDL_Point> curr_path, 
	// 		std::vector<SDL_Point>& end_path, 
	// 		std::set<std::pair<int,int>>& vis){
	// 	curr_path.push_back(pos); // add to path
	// 	vis.insert(std::make_pair(pos.x, pos.y)); // mark as visited

	// 	SDL_Rect r = { LEFT_PANE_W + pos.x * BLOCK_W, pos.y * BLOCK_H, BLOCK_W, BLOCK_H };
	// 	search_markers.push_back(r);	
	// 	
	// 	// std::cout << "Search speed: " << search_speed << "\n";
	// 	// render the current search
	// 	// SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 128);
	// 	// SDL_RenderFillRect(renderer, &rect);
	// 	PathFinder::delayHighRes(search_delay);
	// 	// SDL_RenderPresent(renderer);


	// 	if (pos.x == goal.x && pos.y == goal.y){
	// 		end_path = curr_path;
	// 		// remove starter node in path (player is already on here)
	// 		if (end_path.size() > 0)
	// 			end_path.erase(end_path.begin());
	// 		return;	
	// 	}
	// 	for (const SDL_Point& moves : s_moves){
	// 		int nx, ny;
	// 		nx = pos.x + moves.x, ny = pos.y + moves.y;
	// 		std::pair<int,int> pr = std::make_pair(nx, ny);
	// 		if (world.inBounds(nx, ny) &&
	// 				(world.getPos(nx, ny) == ENT_NONE || world.getPos(nx, ny) == ENT_END) 
	// 				&& vis.find(pr) == vis.end()){
	// 			SDL_Point next_pos = { nx, ny };
	// 			auto future = std::async(std::launch::async,
	// 					dfs_helper, next_pos, std::ref(world), curr_path, std::ref(end_path), std::ref(vis));
	// 			// immediately end the search if we already found a path
	// 			if (end_path.size() > 0)
	// 				return;
	// 		}
	// 	}
	// };

	// find the path
	// auto future = std::async(std::launch::async, 
	// 		dfs_helper, start, std::ref(world), temp_path, std::ref(path), std::ref(vis));

	// animate the path reconstruction formed
	// SDL_Color c_finish = Color::Light::GREEN;
	// SDL_SetRenderDrawColor(renderer, c_finish.r, c_finish.g, c_finish.b, 128);
	
	// for (auto itr = path.rbegin(); itr != path.rend(); ++itr){
	// 	const SDL_Point pt = *itr;
	// 	// const SDL_Rect rect = { LEFT_PANE_W + pt.x * BLOCK_W, pt.y * BLOCK_H, BLOCK_W, BLOCK_H };
	// 	// SDL_RenderFillRect(renderer, &rect);
	// 	SDL_Delay(7);
	// 	// SDL_RenderPresent(renderer);
	// }

	return path;
}
