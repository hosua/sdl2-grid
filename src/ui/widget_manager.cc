#include "widget_manager.hh"

#include <algorithm>
#include <initializer_list>

namespace UI {

	void WidgetManager::addWidget(std::unique_ptr<IWidget> widget){ 
		_widgets.push_back(std::move(widget));	
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
