/*
Pong Game, Made by Nguyen Nguyen
Libraries used:
SDL2
JSON by nlohmann

WARINING: DO NOT MODIFY THE SOURCE CODE UNLESS YOU KNOW WHAT YOU ARE DOING.

Note:
You can actually modify textures and fonts.
Just go to the "res" folder inside the directory.
"gfx" is Graphics.
"sfx" is Sound Effect(s).
"font" is just simply, font.
You can replace the file with another file that you wish to change. 

Make sure the name and extension is as same as the old file.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <nlohmann/json.hpp>
#include "MainWindow.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#define WIDTH 640 //Screen width
#define HEIGHT 460 //Screen height

using namespace std;
using json = nlohmann::json;

int main(int argc, char* argv[]){
    ifstream json_file;
    json_file.open("res/settings.json");

    json json = json::parse(json_file);

    string BALL_DIR = json["BALL_DIR"];
    string PADDLE_DIR  = json["PADDLE_DIR"];
    string FONT_DIR = json["FONT_DIR"];

    const int BALL_SPEED = json["BALL_SPEED"];
    const int PADDLE_SPEED = json["PADDLE_SPEED"];

    short int BALL_SIZE = json["BALL_SIZE"];
    short int PADDLE_WIDTH = json["PADDLE_WIDTH"];
    short int PADDLE_HEIGHT = json["PADDLE_HEIGHT"];

    bool start_screen = true;
    bool run = true;
    bool pause_screen = false;
    SDL_Event event;
    int ball_x_direction = 1;
    int ball_y_direction = 1;
    short int score1 = 0;
    short int score2 = 0;
    string y, z;

    y  = to_string(score1); //convert them to string since the text function
    z = to_string(score2); // only supports string.

    if (SDL_Init(SDL_INIT_VIDEO) > 0){
		std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }
	if (!(IMG_Init(IMG_INIT_PNG))){
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
    }

    #ifdef _WIN32
        FreeConsole();
    #endif

    MainWindow window("Pong", WIDTH, HEIGHT);

    TTF_Init();

    //Font
    TTF_Font * font = TTF_OpenFont(FONT_DIR.c_str(), 40);
    TTF_Font * BigFont = TTF_OpenFont(FONT_DIR.c_str(), 120);

    SDL_Color White = {255, 255, 255, 255};

    ///Welcome text
    SDL_Surface * welcome = TTF_RenderText_Blended(BigFont, "Pong!", White);
    SDL_Surface * text2 = TTF_RenderText_Blended(font, "Press Space to play", White);

    //Score
    SDL_Surface * surface_score1txt = TTF_RenderText_Blended(font, y.c_str(), White); //Load text
    SDL_Surface * surface_score2txt = TTF_RenderText_Blended(font, z.c_str(), White);

    //Pause text
    SDL_Surface * paused = TTF_RenderText_Blended(BigFont, "Paused", White);

    //Welcome Text
    SDL_Texture * WLCM = window.Font_Text_Load(welcome);
    SDL_Rect WLCM_loc;
    TTF_SizeText(BigFont, "Pong!", &WLCM_loc.w, &WLCM_loc.h);
    WLCM_loc.x = WIDTH-WLCM_loc.w-203;
    WLCM_loc.y = HEIGHT/3;

    //Pause Text
    SDL_Texture * PAUSE = window.Font_Text_Load(paused);
    SDL_Rect PAUSE_loc;
    TTF_SizeText(BigFont, "Paused", &PAUSE_loc.w, &PAUSE_loc.h);
    PAUSE_loc.x = WIDTH-PAUSE_loc.w-160;
    PAUSE_loc.y = HEIGHT/3;

    //Control Text
    SDL_Texture * T2 = window.Font_Text_Load(text2);
    SDL_Rect T2_loc;
    TTF_SizeText(font, "Press Space to play", &T2_loc.w, &T2_loc.h);
    cout << T2_loc.w;
    T2_loc.x = WIDTH-T2_loc.w-173;
    T2_loc.y = HEIGHT/1.5;

    //Score 1 Text
    SDL_Texture* ST1 =  window.Font_Text_Load(surface_score1txt); //Convert text to surface so that it can be displayed
    SDL_Rect ST1_loc;
    TTF_SizeText(font, y.c_str(), &ST1_loc.w, &ST1_loc.h); //Change the width and height of the text to be the same as the font size
    ST1_loc.x = WIDTH/2-(12+ST1_loc.w);
    ST1_loc.y = 5;
    
    //Score 2 Text
    SDL_Texture* ST2 =  window.Font_Text_Load(surface_score2txt);
    SDL_Rect ST2_loc;
    TTF_SizeText(font, z.c_str(), &ST2_loc.w, &ST2_loc.h);
    ST2_loc.x = WIDTH/2+12;
    ST2_loc.y = 5;
    

    SDL_Texture *ball = window.texture_load(BALL_DIR.c_str());

    SDL_Rect ball_location;
    ball_location.x = WIDTH/2-BALL_SIZE/2;
    ball_location.y = HEIGHT/2;
    ball_location.h = BALL_SIZE;
    ball_location.w = BALL_SIZE;

    //Paddle 1
    SDL_Texture *bar1 = window.texture_load(PADDLE_DIR.c_str());

    SDL_Rect bar1_location;
    bar1_location.x = 0;
    bar1_location.y = HEIGHT/2;
    bar1_location.h = PADDLE_HEIGHT;
    bar1_location.w = PADDLE_WIDTH;

    //Paddle 2
    SDL_Texture *bar2 = window.texture_load(PADDLE_DIR.c_str());

    SDL_Rect bar2_location;
    bar2_location.x = WIDTH-PADDLE_WIDTH;
    bar2_location.y = HEIGHT/2;
    bar2_location.h = PADDLE_HEIGHT;
    bar2_location.w = PADDLE_WIDTH;

    //Separator
    SDL_Texture *Bar3 = window.texture_load(PADDLE_DIR.c_str());
    SDL_Rect Bar3_location;
    Bar3_location.x = WIDTH/2;
    Bar3_location.y = 0;
    Bar3_location.h = HEIGHT;
    Bar3_location.w = 3;

    while(run){
        if (start_screen == true){
            window.render_load(WLCM, NULL, &WLCM_loc);
            window.render_load(T2, NULL, &T2_loc);
            window.render_display();
        }
        if (pause_screen == true){
            window.clear_render(); //clear screen
            window.render_load(PAUSE, NULL, &PAUSE_loc);
            window.render_load(T2, NULL, &T2_loc);
            window.render_display();
        }
        if (start_screen == false && pause_screen == false){

            ball_location.x += BALL_SPEED * ball_x_direction;
            ball_location.y += BALL_SPEED * ball_y_direction;

            if (ball_location.x <= 0) {
                ball_x_direction = 1;
                score2+=1;

                SDL_DestroyTexture(ST2);
                z = to_string(score2);
                surface_score1txt = TTF_RenderText_Solid(font, z.c_str(), White); //Re-render the text
                ST2 =  window.Font_Text_Load(surface_score1txt);

                TTF_SizeText(font, z.c_str(), &ST2_loc.w, &ST2_loc.h); //Update size

                window.render_load(ST2, NULL, &ST2_loc);

            } else if (ball_location.x + BALL_SIZE >= WIDTH) {
                ball_x_direction = -1;
                score1+=1;

                SDL_DestroyTexture(ST1);
                y  = to_string(score1);
                surface_score1txt = TTF_RenderText_Solid(font, y.c_str(), White);
                ST1 =  window.Font_Text_Load(surface_score1txt);

                TTF_SizeText(font, y.c_str(), &ST1_loc.w, &ST1_loc.h);
                
                if (score1 == 10 || score1 == 100 || score1 == 1000 || score1 == 10000){
                    ST1_loc.x = WIDTH/2-(12+ST1_loc.w); //Decreases text "x" position to the left
                }

                window.render_load(ST1, NULL, &ST1_loc);
            }

            //Some physics
            if (ball_location.y <= 0) {
                ball_y_direction = 1;
            } else if (ball_location.y + 21 >= HEIGHT) {
                ball_y_direction = -1;
            }

            if (SDL_HasIntersection(&ball_location, &bar1_location)){
                //ball_location.x = 21;
                ball_x_direction = 1;
            }
            else if (SDL_HasIntersection(&ball_location, &bar2_location)){
                //ball_location.x = WIDTH-42;
                ball_x_direction = -1;
            }
        }

        //Add control to the paddle

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                run = false;
            }
            if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym){
                case SDLK_w:
                    bar1_location.y -= PADDLE_SPEED;
                    if (bar1_location.y < 0){
                        bar1_location.y = 0;
                    }
                    break;
                
                case SDLK_s:
                    bar1_location.y += PADDLE_SPEED;
                    if (bar1_location.y > HEIGHT-PADDLE_HEIGHT){
                        bar1_location.y = HEIGHT-PADDLE_HEIGHT;
                    }  
                    break;
                
                case SDLK_UP:
                    bar2_location.y -= PADDLE_SPEED;
                    if (bar2_location.y < 0){
                        bar2_location.y = 0;
                    }
                    break;
                
                case SDLK_DOWN:
                    bar2_location.y += PADDLE_SPEED;
                    if (bar2_location.y > HEIGHT-PADDLE_HEIGHT){
                        bar2_location.y = HEIGHT-PADDLE_HEIGHT;              
                    }
                    break;
                case SDLK_SPACE:
                    if (start_screen == true){
                        window.clear_render();
                        start_screen = false;
                        continue;
                    }
                    else if (pause_screen == false){
                        pause_screen = true;
                        continue;
                        
                    }
                    if (pause_screen == true){
                        pause_screen = false;
                        continue;
                    }
                    break;
                }
            }
        }
        
        //Refresh the screen
        if (start_screen == false && pause_screen == false){
            window.clear_render();
            window.render_load(ST1, NULL, &ST1_loc);
            window.render_load(ST2, NULL, &ST2_loc);
            window.render_load(ball, NULL, &ball_location);
            window.render_load(bar1, NULL, &bar1_location);
            window.render_load(bar2, NULL, &bar2_location);
            window.render_load(Bar3, NULL, &Bar3_location);
            window.render_display();
        }
    }
    	
    TTF_CloseFont(font);
    TTF_CloseFont(BigFont);
    SDL_DestroyTexture(WLCM);
    SDL_DestroyTexture(ball);
    SDL_DestroyTexture(bar1);
    SDL_DestroyTexture(bar2);
    SDL_DestroyTexture(Bar3);
    SDL_DestroyTexture(ST1);
    SDL_DestroyTexture(ST2);
    SDL_DestroyTexture(T2);
    window.cleanup();
    SDL_Quit();
    return 0;
}