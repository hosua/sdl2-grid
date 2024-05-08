#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
#include <vector>
#include "defs.hh"


namespace UI {
	class IWidget { // Interface for all UI objects
		public:
			IWidget(int x, int y, SDL_Renderer* &renderer);
			~IWidget() = default;
			virtual void render() = 0;
			virtual void handleInputs(SDL_Event event);
			
			bool isMouseOver() const;
			bool isMouseScrolledUp(SDL_Event event) const;
			bool isMouseScrolledDown(SDL_Event event) const;
			bool isClicked(SDL_Event event) const;
			uint32_t getID() const;

		protected:
			SDL_Rect _rect; // destination rect
			SDL_Renderer* &_renderer;
			const uint32_t _id;
	};

	class WidgetManager {
	public:
		~WidgetManager() = default;
		WidgetManager();
		
		bool addWidget(std::unique_ptr<IWidget> widget); // true if add sucessful
		bool removeWidget(uint32_t id); // true if remove successful

		void renderWidgets();
		void handleWidgetInputs(SDL_Event event);

	private:
		std::vector<std::unique_ptr<IWidget>> _widgets;
		int _widget_count = 0;
	};

	class Text : IWidget {
		public:
			~Text();
			Text(const std::string& text, 
					int x, int y, 
					SDL_Renderer* &renderer,
					TTF_Font* font
				);

			SDL_Rect getSize();
			void setPos(int x, int y);
			void setText(const std::string& text);
			void render() override;
		private:
			std::string _text;	
			SDL_Surface* _surface;
			SDL_Texture* _texture;
			TTF_Font* _font;
			SDL_Renderer* &_renderer;
	};

	class Button : public IWidget {
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
			
			// override handleInputs from IWidget class for input events
			// void handleInput(SDL_Point mouse_pos, const uint8_t* kb_state) override;
			void render() override;
		private:
			Text _text;
			SDL_Color _bg_color, _hover_color;
	};

	typedef enum {
		ST_VERTICAL,
		ST_HORIZONTAL,
	} SpinnerType;
	
	// TODO: I really want to be able to support all arithmetic types (uint8_t,
	// short, long, etc). However, I can't get the implementation to work. For
	// now, casting any of these types should work fine but look into this
	// again later.
	template<typename T>
	class Spinner : public IWidget {
		public:
			~Spinner() = default;
			Spinner(T& val,
					int x, int y,
					int w, int h,
					T min_val, T max_val, T interval,
					SDL_Renderer* &renderer,
					SpinnerType spinner_type,
					TTF_Font* font = Font::openSansSmall,
					SDL_Color bg_color = Color::GREY,
					SDL_Color btn_color = Color::DARK_GREY,
					SDL_Color btn_hover_color = Color::LIGHT_GREY);

			SDL_Rect getSize();
			void setPos(int x, int y);
			void incVal();
			void decVal();

			void render() override;
			void handleInputs(SDL_Event event) override;
			
		private:
			Text _text;
			T _val, _min_val, _max_val, _interval;
			SDL_Color _bg_color, _btn_color, _btn_hover_color;
			std::unique_ptr<Button> _inc_btn, _dec_btn;
			SpinnerType _type; 
	};
}
