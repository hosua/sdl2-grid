#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>

namespace PathFinder {

	// The entity types that can be placed in the world
	typedef enum {
		ENT_NONE,
		ENT_END,
		ENT_WALL,
		ENT_PLAYER,
	} EntType;

	const extern std::unordered_map<int, float> SEARCH_SPEED_MAP;
	// The mapping for each entity and what color they will be rendered as
	extern std::unordered_map<EntType, SDL_Color> ENT_COLOR_MAP;

	class World {
	public:
		World(int x, int y,
				int w, int h);

		~World() = default;
		void draw();

		bool movePlayer(int x, int y);  /*!< Moves player directly to (x,y). Returns false if unmoved */
		
		/** Moves player relative to its current position.
		 *
		 * TODO:
		 * \bug movePlayerRelative() currently does not properly check for
		 * collisions if the player moves by more than 1 tile per call.
		 */
		bool movePlayerRelative(int dx, int dy); 
		
		SDL_Point getEndPos() const { return _end; }
		void setEndPos(int x, int y){ _end = { x, y }; }

		SDL_Point getPlayerPos() const { return _player; }
		const SDL_Rect getRect() const { return _rect; }

		// clearly renders only the world grid portion of the screen
		void renderClear();

		EntType getEntityAt(int x, int y) const; // gets the entity type at the specified position

		void spawnEntity(const EntType entity_type, int x, int y);
		void deleteWall(int x, int y); // walls are currently the only type that should be deletable

		bool inBounds(int x, int y) const { return (x >= 0 && x < _cols && y >= 0 && y < _rows); }

		const std::pair<int,int> getDimensions();

		std::vector<std::vector<EntType>> const getGrid(){ return _grid; }
		std::vector<std::vector<EntType>> setGrid(std::vector<std::vector<EntType>> grid){ 
			_grid = grid; 
			return grid; 
		}

	private:
		uint16_t _rows, _cols;
		std::vector<std::vector<EntType>> _grid;
		SDL_Rect _rect;	
		SDL_Point _end, _player;
	};

	void randomize_world(World& world);
}
