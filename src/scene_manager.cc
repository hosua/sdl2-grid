#include "scene_manager.hh"
#include <algorithm>


SceneManager::SceneManager(SDL_Renderer* &renderer): _renderer(renderer) {};	

void SceneManager::addScene(std::unique_ptr<Scene> scene){
	_scenes.push_back(std::move(scene));
};

bool SceneManager::removeScene(size_t index){
	if (index >= _scenes.size())
		return false;
	_scenes.erase(std::next(_scenes.begin(), index));
	return true;
};

bool SceneManager::removeScene(const std::string& key){
	auto itr = std::remove_if(_scenes.begin(), _scenes.end(), 
			[&](const std::unique_ptr<Scene>& scene){
				return key == scene->getKey();
			});
	if (itr == _scenes.end())
		return false;
	_scenes.erase(itr);
	return true;
};

bool SceneManager::renderScenes(){
	SDL_Point mouse_pos;
	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	drawClear();
	for (auto itr = _scenes.rbegin(); itr != _scenes.rend(); ++itr){
		Scene* scene = itr->get();
		if (!scene->render(_renderer))
			return false;
		scene->handleInputs(mouse_pos);
	}
	drawPresent();
	return true;
};

void SceneManager::drawClear(const SDL_Color& color) const {
	const SDL_Color& c = color;
	SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
	SDL_RenderClear(_renderer);
}

void SceneManager::drawPresent() const {
	SDL_RenderPresent(_renderer);
}

