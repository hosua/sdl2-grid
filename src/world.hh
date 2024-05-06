#pragma once

#include <vector>

#include "entity.hh"
#include "defs.hh"

extern const int WORLD_W;
extern const int WORLD_H;

class World {
public:
	World();
	~World() = default;
	void draw(SDL_Renderer* renderer);
	void movePlayer(int x, int y);
	SDL_Point getStartPos();
	SDL_Point getEndPos();
	SDL_Point getPlayerPos();

	void spawnWall(int x, int y);
	void deleteWall(int x, int y);

private:
	uint16_t _rows, _cols;
	std::vector<std::vector<EntType>> _grid;
	bool inBounds(int x, int y);
	SDL_Point _start, _end, _player;
};
