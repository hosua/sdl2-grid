#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "widget.hh"

namespace UI {

	class WidgetManager {
	public:
		~WidgetManager() = default;
		WidgetManager();
		
		bool addWidget(std::unique_ptr<IWidget> widget); // true if add sucessful
		bool removeWidget(uint32_t id); // true if remove successful

		void renderWidgets();
		void handleWidgetInputs();

	private:
		std::vector<std::unique_ptr<IWidget>> _widgets;
		int _widget_count = 0;
	};
}
