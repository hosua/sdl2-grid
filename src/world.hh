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
	SDL_Point getStartPos() const;
	SDL_Point getEndPos() const;
	SDL_Point getPlayerPos() const;

	bool getPlayerMoveFlag() const;
	void setPlayerMoveFlag(bool flag);

	EntType getPos(int x, int y) const; // gets the entity type at the specified position

	void spawnWall(int x, int y);
	void deleteWall(int x, int y);

	bool inBounds(int x, int y) const;

private:
	uint16_t _rows, _cols;
	std::vector<std::vector<EntType>> _grid;
	SDL_Point _start, _end, _player;
	bool _player_move_flag; // if true, path rendering should stop
};
