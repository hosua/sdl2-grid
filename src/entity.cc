#include "entity.hh"
#include "defs.hh"

std::unordered_map<EntType, SDL_Color> ENT_COLOR_MAP = {
	{ ENT_NONE, Color::BLACK },
	{ ENT_START, Color::GREEN },
	{ ENT_END, Color::RED },
	{ ENT_WALL, Color::WHITE },
	{ ENT_PLAYER, Color::BLUE },
};
