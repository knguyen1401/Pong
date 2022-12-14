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
		void RenderMiddleX(SDL_Texture * txtr, int SCREEN_WIDTH, int SPRITE_WIDTH, SDL_Rect * srcrect, SDL_Rect dstrect);
		void RenderMiddleY(SDL_Texture * txtr, int SCREEN_HEIGHT, int SPRITE_HEIGHT, SDL_Rect * srcrect, SDL_Rect dstrect);
		void RenderMiddleScreen(SDL_Texture * txtr, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SPRITE_HEIGHT, int SPRITE_WIDTH, SDL_Rect * srcrect, SDL_Rect dstrect);

		SDL_Texture* TextureLoadFromFile(const char* filepath);
		SDL_Texture * LoadText(TTF_Font *font, const char *text, SDL_Color fg);
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Surface * TextSurface;
		SDL_Texture * Texture;
};