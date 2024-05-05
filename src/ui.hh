#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "defs.hh"

namespace UI {
	class Manager;
	class Widget;

	class Widget { // UI element base class
		public:
			Widget(int x, int y, SDL_Renderer* &renderer);
			~Widget() = default;
			virtual void render(SDL_Renderer* renderer) = 0;
			virtual void handleInput(SDL_Point mouse_pos, const uint8_t* kb_state, SDL_Event& e);

			uint32_t getID();

		protected:
			SDL_Rect _rect; // destination rect
			SDL_Renderer* &_renderer;
			bool _mouse_over; // true if mouse hovering widget
			const uint32_t _id;
	};
	
	// UI Manager
	class Manager {
		public:
			Manager(SDL_Event& event);
			~Manager() = default;

			bool addWidget(Widget* widget);
			bool removeWidget(uint32_t id);
			void renderAndHandleWidgets(SDL_Point mouse_pos, const uint8_t* kb_state, SDL_Renderer* renderer);
		private:

			uint32_t _widget_count = 0; // widget counter used to assign unique IDs to each widget
			std::vector<Widget*> _widgets; // the list of widgets
			SDL_Event& _event;
	};

	class Window {

	};

	class Text : Widget {
		public:
			Text(const std::string& text, 
					int x, int y, 
					SDL_Renderer* renderer,
					TTF_Font* font
				);

			SDL_Rect getSize();
			void setPos(int x, int y);

			~Text();
			void render(SDL_Renderer* renderer);
		private:
			std::string _text;	
			SDL_Surface* _surface;
			SDL_Texture* _texture;
	};

	class Button : public Widget {
		public:
			Button(const std::string& text, 
					int x, int y, 
					int w, int h,
					SDL_Renderer* renderer,
					TTF_Font* font = Font::openSansMedium,
					SDL_Color bg_color = Color::GREY,
					SDL_Color hover_color = Color::LIGHT_GREY
				  );

			~Button() = default;

			void handleInput(SDL_Point mouse_pos, const uint8_t* kb_state, SDL_Event& e) override;
			void render(SDL_Renderer* renderer) override;

		private:

			Text _text;
			SDL_Color _bg_color, _hover_color;
	};
}
