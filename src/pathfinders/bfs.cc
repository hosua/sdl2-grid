#include "bfs.hh"

#include <map>
#include <set>
#include <utility>
#include <queue>
#include <functional>
#include <vector>

static std::vector<SDL_Point> s_moves = {{0, +1}, {+1, 0}, {-1, 0}, {0, -1}};

std::vector<SDL_Point> bfs(World& world, SDL_Renderer* &renderer){
	using std::vector, std::function,
		  std::map, std::pair, std::make_pair;

	vector<SDL_Point> path;
	std::set<pair<int,int>> vis;
	map<pair<int,int>, SDL_Point> parent;

	function<void(SDL_Point, World&, vector<SDL_Point>, vector<SDL_Point>&,
			std::set<pair<int,int>>&)> bfs_helper;

	SDL_Point start = world.getPlayerPos();
	SDL_Point goal = world.getEndPos();

	std::queue<SDL_Point> q; // <curr_node, parent>
	q.push(start);
	parent[make_pair(start.x, start.y)] = {-1,-1};

	std::vector<SDL_Rect> search_markers;


	const SDL_Color c = Color::GREEN;
	while (!q.empty()){
		size_t breadth = q.size();
		while (breadth--){
			SDL_Point pos = q.front();
			printf("(%i,%i) -> ", pos.x, pos.y);
			q.pop();
			SDL_Rect rect = { LEFT_PANE_W + pos.x * BLOCK_W, pos.y * BLOCK_H, BLOCK_W, BLOCK_H };
			const SDL_Rect* rects = &search_markers[0];
			search_markers.push_back(rect);

			world.renderClear(renderer);
			world.draw(renderer);

			// animate & reconstruct the path we formed when we reach the goal
			if (pos.x == goal.x && pos.y == goal.y){
				SDL_Color c_finish = Color::LIGHT_GREEN;
				SDL_SetRenderDrawColor(renderer, c_finish.r, c_finish.g, c_finish.b, 128);
				// reconstruct
				pair<int,int> crawl = make_pair(pos.x, pos.y);
				SDL_Point c = { crawl.first, crawl.second };
				path.push_back(c);
				while (crawl.first != start.x || crawl.second != start.y){
					c = { crawl.first, crawl.second };
					path.push_back(c);
					SDL_Point p = parent[crawl];
					crawl.first = p.x, crawl.second = p.y;
					rect = { LEFT_PANE_W + p.x * BLOCK_W, p.y * BLOCK_H, BLOCK_W, BLOCK_H };
					SDL_RenderFillRect(renderer, &rect);
					SDL_Delay(5); // add some delay to the animation
					SDL_RenderPresent(renderer);
				}

				// remove starter node in path (player is already on here)
				if (path.size() > 0)
					path.erase(path.begin());
				std::reverse(path.begin(), path.end());
				return path;
			}

			for (const SDL_Point& moves : s_moves){
				SDL_Point n = {pos.x + moves.x, pos.y + moves.y};
				pair<int,int> pr = make_pair(n.x, n.y);
				if (world.inBounds(n.x, n.y) &&
						(world.getPos(n.x, n.y) == ENT_NONE || world.getPos(n.x, n.y) == ENT_END) 
						&& vis.find(pr) == vis.end()){
					q.push(n);
					parent[make_pair(n.x, n.y)] = pos;
					vis.insert(make_pair(n.x, n.y)); // mark as visited
				}
			}

			// render the current search
			SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 128);
			SDL_RenderFillRects(renderer, rects, search_markers.size());

			SDL_Delay(1); // add some delay to the animation
			SDL_RenderPresent(renderer);

		}
	}


	return {}; // no path found
}

