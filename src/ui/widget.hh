#pragma once

#include <SDL2/SDL.h>

namespace UI {
	class IWidget { // Interface for all UI objects
		public:
			IWidget(int x, int y);
			~IWidget() = default;
			virtual void render() = 0;
			virtual void handleInputs();
			
			bool isMouseOver() const;
			bool isMouseScrolledUp() const;
			bool isMouseScrolledDown() const;
			bool isClicked() const;
			uint32_t getID() const;

		protected:
			SDL_Rect _rect; // destination rect
			const uint32_t _id;
	};
}
