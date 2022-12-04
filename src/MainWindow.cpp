#include <iostream>
#include <SDL2/SDL.h>
#include "MainWindow.hpp"
#include <SDL2/SDL_ttf.h>

MainWindow::MainWindow(const char* title, int width, int height){
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
	{
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void MainWindow::cleanup(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void MainWindow::clear_render(){
    SDL_RenderClear(renderer);
}

SDL_Texture* MainWindow::texture_load(const char* filename){
    SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
	return texture;
}

SDL_Texture* MainWindow::Font_Text_Load(SDL_Surface* msg){
    SDL_CreateTextureFromSurface(renderer, msg);
}

void MainWindow::render_load(SDL_Texture* txtr, const SDL_Rect * srcrect, const SDL_Rect * dstrect){
    SDL_RenderCopy(renderer, txtr, srcrect, dstrect);
}

void MainWindow::render_display(){
    SDL_RenderPresent(renderer);
}
