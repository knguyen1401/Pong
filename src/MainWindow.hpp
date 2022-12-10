#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class MainWindow {
	public:
		MainWindow(const char* title, int width, int height);
		void Cleanup();
		void ClearRender();
		void RenderDisplay();
		void RenderLoad(SDL_Texture * txtr, const SDL_Rect * srcrect, const SDL_Rect * dstrect);
		SDL_Texture* TextureLoadFromFile(const char* filepath);
		SDL_Texture* TextLoad(SDL_Surface* msg);
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
};