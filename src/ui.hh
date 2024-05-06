#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
#include <vector>
#include "defs.hh"

namespace UI {
	class Widget { // UI element base class
		public:
			Widget(int x, int y, SDL_Renderer* &renderer);
			~Widget() = default;
			virtual void render() = 0;
			virtual void handleInputs(SDL_Event event);
			
			bool isMouseOver();
			uint32_t getID();

		protected:
			SDL_Rect _rect; // destination rect
			SDL_Renderer* &_renderer;
			const uint32_t _id;
	};

	class WidgetManager {
	public:
		~WidgetManager() = default;
		WidgetManager();
		
		bool addWidget(std::unique_ptr<Widget> widget); // true if add sucessful
		bool removeWidget(uint32_t id); // true if remove successful

		void renderWidgets();
		void handleWidgetInputs(SDL_Event event);

	private:
		std::vector<std::unique_ptr<Widget>> _widgets;
		int _widget_count = 0;
	};

	class Text : Widget {
		public:
			~Text();
			Text(const std::string& text, 
					int x, int y, 
					SDL_Renderer* &renderer,
					TTF_Font* font
				);

			SDL_Rect getSize();
			void setPos(int x, int y);

			void render();
		private:
			std::string _text;	
			SDL_Surface* _surface;
			SDL_Texture* _texture;
	};

	
	// TODO: Maybe make this a template to be usable with floats & doubles latewr
	class Spinner : Widget {
		public:
			~Spinner() = default;
			Spinner(int& val,
					int x, int y,
					int w, int h,
					SDL_Renderer* &renderer,
					int min_val = 0, int max_val = 10,
					TTF_Font* font = Font::openSansMedium,
					SDL_Color bg_color = Color::DARK_GREY,
					SDL_Color hover_color = Color::LIGHT_GREY);

			SDL_Rect getSize();
			void setPos(int x, int y);
			void incVal();
			void decVal();

		private:
			Text _text;
			int _val, _min_val, _max_val;
	};

	class Button : public Widget {
		public:
			Button(const std::string& text, 
					int x, int y, 
					int w, int h,
					SDL_Renderer* &renderer,
					TTF_Font* font = Font::openSansMedium,
					SDL_Color bg_color = Color::DARK_GREY,
					SDL_Color hover_color = Color::LIGHT_GREY
				  );

			~Button() = default;
			
			// override handleInputs from Widget class for input events

			void render();
			// void handleInput(SDL_Point mouse_pos, const uint8_t* kb_state) override;
		private:
			Text _text;
			SDL_Color _bg_color, _hover_color;
	};

}
