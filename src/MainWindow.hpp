#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class MainWindow {
	public:
		MainWindow(const char* title, int width, int height);
		void cleanup();
		void clear_render();
		void render_display();
		void render_load(SDL_Texture * txtr, const SDL_Rect * srcrect, const SDL_Rect * dstrect);
		SDL_Texture* texture_load(const char* filepath);
		SDL_Texture* Font_Text_Load(SDL_Surface* msg);
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
};