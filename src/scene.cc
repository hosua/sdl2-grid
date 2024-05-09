#include "scene.hh"
#include <algorithm>

IScene::IScene(const std::string key, SDL_Renderer* &renderer,
			bool is_handling_inputs, bool is_rendering):
	_is_handling_inputs(is_handling_inputs),
	_is_rendering(is_rendering),
	_key(key), 
	_renderer(renderer) {}

