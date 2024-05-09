#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "widget.hh"

namespace UI {

	class WidgetManager {
	public:
		~WidgetManager() = default;
		WidgetManager() = default;
		
		void addWidget(std::unique_ptr<IWidget> widget); 
		bool removeWidget(uint32_t id); // true if remove successful

		void renderWidgets();
		void handleWidgetInputs();

	protected:
		std::vector<std::unique_ptr<IWidget>> _widgets; 		/*!< The list of widgets to render for the associated Scene */
	};
}
