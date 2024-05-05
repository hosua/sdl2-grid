#include "scene.hh"
#include <algorithm>

Scene::Scene(const std::string& key): _key(key){};

const std::string& Scene::getKey() const { 
	return _key;
}
