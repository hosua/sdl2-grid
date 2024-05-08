#include "pathfinders/defs.hh"

#include <chrono>
#include <iostream>

std::unordered_map<int, float> PathFinder::SEARCH_SPEED_MAP = {
	{0,  (int)18.0},
	{1,  (int)16.0},
	{2,  (int)14.0},
	{3,  (int)12.0},
	{4,  (int)10.0},
	{5,  (int)8.0},
	{6,  (int)6.0},
	{7,  (int)4.0},
	{8,  (int)2.0},
	{9,  (int)1.0},
	{10, (int)0.5},
};

void PathFinder::delayHighRes(float ms){
	int ns = (ms * (float)1e6);
	auto start_time = std::chrono::steady_clock::now();
	while ((std::chrono::steady_clock::now() - start_time) < std::chrono::nanoseconds(ns))
		continue;
}
