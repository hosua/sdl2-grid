#include "astar.hh"

#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <queue>
#include "../defs.hh"

static std::vector<SDL_Point> s_moves = {{0, +1}, {+1, 0}, {-1, 0}, {0, -1}};

static float get_euclidean_dist(int x1, int y1, int x2, int y2){
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

// g = cost of path from start to current pos (number of parent nodes)
// h = heuristic estimate of the cost to reach the goal (manhattan dist? Euclidean dist?)
// f = g + h
struct SearchState {
	int g, h, f;
	int x, y;
	std::shared_ptr<SearchState> parent;
};

// (for debugging) print the search state info
std::ostream & operator << (std::ostream &out, const SearchState& s){
	out << '[' << s.f << ',' << s.g << ',' << s.h << "]: (" <<
		s.x << ',' << s.y << ") -> ";
	return out;
}

// necessary for priority_queue insertion and sorting
bool operator<(const SearchState& a, const SearchState& b){
	return a.f > b.f; // min heap on f = g + h
}

class AStarPriorityQueue : public std::priority_queue<SearchState> {
	public:
		std::shared_ptr<SearchState> find(const int x, const int y) {
			for (auto itr = c.begin(); itr != c.end(); ++itr){
				SearchState state = *itr;
				if (x == state.x && y == state.y)
					return std::make_shared<SearchState>(*itr);
			}
			return nullptr;
		}
};

std::vector<SDL_Point> PathFinder::a_star(World& world){
	std::vector<SDL_Point> path;
	
	int g_count = 0; 

	SDL_Point start = world.getPlayerPos();
	SDL_Point goal = world.getEndPos();

	int start_h = floor(get_euclidean_dist(start.x, start.y, goal.x, goal.y));
	
	SearchState start_node = {
		g_count,
		start_h,
		g_count + start_h,
		start.x, start.y,
		nullptr
	};

	g_count++;

	std::set<std::pair<int,int>> closed;

	AStarPriorityQueue pq;
	pq.push(start_node);

	while (!pq.empty()){
		SearchState node = pq.top();	
		pq.pop();

		std::cout << node;
		int x = node.x, y = node.y;
		closed.insert(std::make_pair(x, y));
		if (x == goal.x && y == goal.y){
			std::cout << "\nReconstructing the path...\n";
			// crawl
			for (SearchState* crawl = node.parent.get(); crawl; crawl = crawl->parent.get()){
				// std::cout << '(' << crawl->x << ',' << crawl->y << ")\n";
				path.push_back({ crawl->x, crawl->y });
				std::reverse(path.begin(), path.end());
				return path;

			}
			break;
		}
		
		// "expand" the current node
		for (const auto& [mx, my] : s_moves){
			int nx = x + mx, 
				ny = y + my;
			if (world.inBounds(nx, ny) &&
					world.getEntityAt(nx, ny) != ENT_WALL && // is not blocked
					closed.find(std::make_pair(nx, ny)) == closed.end()){ // has not already been marked closed

				int h = floor(get_euclidean_dist(nx, ny, goal.x, goal.y));
				SearchState new_node = {
					g_count,
					h,
					g_count + h, // f
					nx, ny,
					std::make_shared<SearchState>(node) // parent of the new node is the current node
				};
				
				std::shared_ptr<SearchState> open_node;
				if ( (open_node = pq.find(nx, ny)) ){
					// check update parent of node based on g value
					if (g_count < open_node->g){
						// we found a better g, update the parent of the adjacent node
						open_node->parent = std::make_shared<SearchState>(new_node);

						// recalculate f g h
						open_node->g = g_count;
						open_node->f = open_node->g + open_node->h;

					}
				} else { 
					// did not already exist in the open list, so insert it
					pq.push(new_node);
				}
			}
		}
	}
	std::cout << "No path found\n";
	return {};
}

