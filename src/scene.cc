#include "scene.hh"
#include <algorithm>

IScene::IScene(const std::string& key, SDL_Renderer* &renderer): 
	_key(key), 
	_renderer(renderer) {};

bool IScene::addWidget(std::unique_ptr<UI::IWidget> widget){
	return _widget_mgr.addWidget(std::move(widget));
}

bool IScene::removeWidget(uint32_t id){
	return _widget_mgr.removeWidget(id);
}

const std::string& IScene::getKey() const { 
	return _key;
}

void IScene::renderWidgets(){
	_widget_mgr.renderWidgets();
}

void IScene::handleWidgetInputs(){
	_widget_mgr.handleWidgetInputs();
}


