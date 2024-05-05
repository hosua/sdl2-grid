#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "defs.hh"

namespace UI {
	class Widget { // UI element base class
		public:
			Widget(int x, int y, SDL_Renderer* &renderer);
			~Widget() = default;
			virtual void render() = 0;
			virtual void handleInput(SDL_Point mouse_pos, const uint8_t* kb_state);

			uint32_t getID();

		protected:
			SDL_Rect _rect; // destination rect
			SDL_Renderer* &_renderer;
			bool _mouse_over; // true if mouse hovering widget
			const uint32_t _id;
	};

	bool removeWidget(uint32_t id);

	void renderAndHandleWidgets(SDL_Point mouse_pos, const uint8_t* kb_state);

	class Window {

	};

	class Text : Widget {
		public:
			Text(const std::string& text, 
					int x, int y, 
					SDL_Renderer* &renderer,
					TTF_Font* font
				);

			SDL_Rect getSize();
			void setPos(int x, int y);

			~Text();
			void render();
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
					SDL_Renderer* &renderer,
					TTF_Font* font = Font::openSansMedium,
					SDL_Color bg_color = Color::GREY,
					SDL_Color hover_color = Color::LIGHT_GREY
				  );

			~Button() = default;

			virtual void onClick(void*) = 0;

			void render();
			// void handleInput(SDL_Point mouse_pos, const uint8_t* kb_state) override;
		private:
			Text _text;
			SDL_Color _bg_color, _hover_color;
	};

}
