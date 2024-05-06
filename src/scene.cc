#include "scene.hh"
#include <algorithm>

Scene::Scene(const std::string& key, SDL_Renderer* &renderer): 
	m_renderer(renderer), _key(key) {};

bool Scene::addWidget(std::unique_ptr<UI::Widget> widget){
	return _widget_mgr.addWidget(std::move(widget));
}

bool Scene::removeWidget(uint32_t id){
	return _widget_mgr.removeWidget(id);
}

const std::string& Scene::getKey() const { 
	return _key;
}

void Scene::renderAndHandleWidgetInputs(){
	_widget_mgr.renderAndHandleInputs();
}
