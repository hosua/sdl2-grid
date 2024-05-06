#include "entity.hh"
#include "defs.hh"

std::unordered_map<EntType, SDL_Color> ENT_COLOR_MAP = {
	{ ENT_NONE, Color::BLACK },
	{ ENT_END, Color::RED },
	{ ENT_WALL, Color::GREY },
	{ ENT_PLAYER, Color::BLUE },
};
