#include "widget_manager.hh"

#include <algorithm>

namespace UI {

	WidgetManager::WidgetManager(){}

	bool WidgetManager::addWidget(std::unique_ptr<IWidget> widget){ // true if add sucessful
		_widgets.push_back(std::move(widget));	
		return true;
	} 

	bool WidgetManager::removeWidget(uint32_t id){ // true if remove successful
		auto itr = std::remove_if(_widgets.begin(), _widgets.end(), [&](std::unique_ptr<IWidget>& wg){
				return (wg->getID() == id);
				});
		if (itr == _widgets.end()) return false;

		_widgets.erase(itr, _widgets.end());
		return true;
	} 

	void WidgetManager::renderWidgets(){
		for (std::unique_ptr<IWidget>& wg : _widgets)
			wg->render();
	}

	void WidgetManager::handleWidgetInputs(){
		for (std::unique_ptr<IWidget>& wg : _widgets)
			wg->handleInputs();
	}

}
