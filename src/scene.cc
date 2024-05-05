#include "scene.hh"
#include <algorithm>
#include <iostream>

Scene::Scene(const std::string& key): _key(key){};

const std::string& Scene::getKey() const { 
	return _key;
}

bool Scene::isRendering() const {
	return _is_rendering;
}

bool Scene::isPaused() const {
	return _is_paused;
}

void Scene::setRendering(bool flag){
	if (flag == _is_rendering)
		std::cerr << "Warning: setRendering() used in scene " << _key 
			<< " but it was already " << ((flag) ? "true" : "false");
	_is_rendering = flag;
}

void Scene::setPaused(bool flag){
	if (flag == _is_paused)
		std::cerr << "Warning: setPaused() used in scene " << _key 
			<< " but it was already " << ((flag) ? "true" : "false");
	_is_paused = flag;
}

void Scene::toggleRendering(){
	_is_rendering = !_is_rendering;
}

void Scene::togglePaused(){
	_is_paused = !_is_paused;
}
