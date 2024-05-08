#pragma once

#include <unordered_map>

namespace PathFinder {
	static const std::unordered_map<int, float> SEARCH_SPEED_MAP = {
		{0,  18.0},
		{1,  16.0},
		{2,  14.0},
		{3,  12.0},
		{4,  10.0},
		{5,  8.0 },
		{6,  6.0 },
		{7,  4.0 },
		{8,  2.0 },
		{9,  1.0 },
		{10, 0.5 },
	};

}
