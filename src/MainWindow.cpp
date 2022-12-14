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

void MainWindow::Cleanup(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void MainWindow::ClearRender(){
    SDL_RenderClear(renderer);
}

SDL_Texture* MainWindow::TextureLoadFromFile(const char* filename){
    Texture = IMG_LoadTexture(renderer, filename);
    return Texture;
}

SDL_Texture * MainWindow::LoadText(TTF_Font *font, const char *text, SDL_Color fg){
    TextSurface = TTF_RenderText_Blended(font, text, fg);
    SDL_CreateTextureFromSurface(renderer, TextSurface);
}

void MainWindow::RenderLoad(SDL_Texture* txtr, const SDL_Rect * srcrect, const SDL_Rect * dstrect){
    SDL_RenderCopy(renderer, txtr, srcrect, dstrect);
}

void MainWindow::RenderMiddleX(SDL_Texture * txtr, int SCREEN_WIDTH, int SPRITE_WIDTH, SDL_Rect * srcrect, SDL_Rect dstrect){
    dstrect.x = (SCREEN_WIDTH - SPRITE_WIDTH)/2;
    SDL_RenderCopy(renderer, txtr, srcrect, &dstrect);
}

void MainWindow::RenderMiddleY(SDL_Texture * txtr, int SCREEN_HEIGHT, int SPRITE_HEIGHT, SDL_Rect * srcrect, SDL_Rect dstrect){
    dstrect.y = (SCREEN_HEIGHT - SPRITE_HEIGHT)/2;
    SDL_RenderCopy(renderer, txtr, srcrect, &dstrect);
}

void MainWindow::RenderMiddleScreen(SDL_Texture * txtr, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SPRITE_HEIGHT, int SPRITE_WIDTH, SDL_Rect * srcrect, SDL_Rect dstrect){
    dstrect.x = (SCREEN_WIDTH - SPRITE_WIDTH)/2;
    dstrect.y = (SCREEN_HEIGHT - SPRITE_HEIGHT)/2;
    SDL_RenderCopy(renderer, txtr, srcrect, &dstrect);
}

void MainWindow::RenderDisplay(){
    SDL_RenderPresent(renderer);
}
