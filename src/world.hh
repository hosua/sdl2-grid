#pragma once

#include <vector>

#include "entity.hh"
#include "defs.hh"

extern const int WORLD_W;
extern const int WORLD_H;

class World {
public:
	World(bool& render_path_flag);
	~World() = default;
	void draw(SDL_Renderer* renderer);
	void movePlayer(int x, int y); 
	SDL_Point getStartPos() const; // unused

	SDL_Point getEndPos() const;
	void setEndPos(int x, int y);

	SDL_Point getPlayerPos() const;

	bool getRenderPathFlag() const;
	void setRenderPathFlag(bool flag);

	// clearly renders only the world grid portion of the screen
	void renderClear(SDL_Renderer* renderer);

	EntType getPos(int x, int y) const; // gets the entity type at the specified position

	void spawnEntity(const EntType entity_type, int x, int y);
	void deleteWall(int x, int y); // walls are currently the only type that should be deletable

	bool inBounds(int x, int y) const;

	const std::pair<int,int> getDimensions();
private:
	uint16_t _rows, _cols;
	std::vector<std::vector<EntType>> _grid;
	SDL_Point _start, _end, _player;
	bool& _render_path_flag; // if true, path rendering should stop
};
