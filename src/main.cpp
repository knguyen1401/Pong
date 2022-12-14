/*
Pong Game, Made by Nguyen Nguyen
Built under GCC 12.2.0
Standard: C++17/C++20

Libraries used:
SDL2 version 2.26.0
JSON by nlohmann

SFX used:
Chrome Dino SFX :-)

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

#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <nlohmann/json.hpp>
#include "MainWindow.hpp"

//#ifdef _WIN32
//#include <windows.h>
//#endif

#define WIDTH 800 //Screen width
#define HEIGHT 600 //Screen height

#define JSON_PATH "res/settings.json"

using namespace std;
using json = nlohmann::json;

int main(int argc, char* argv[]){
    ifstream json_file;
    json_file.open(JSON_PATH);

    json json = json::parse(json_file);

    //Load data from JSON
    string BALL_DIR = json["BALL_DIR"];
    string PADDLE_DIR  = json["PADDLE_DIR"];
    string FONT_DIR = json["FONT_DIR"];

    string HIT_SFX_DIR = json["HIT_SFX_DIR"];
    string SCORE_SFX_DIR = json["SCORE_SFX_DIR"];

    const short int BALL_SPEED = json["BALL_SPEED"];
    const short int PADDLE_SPEED = json["PADDLE_SPEED"];

    const short int BALL_SIZE = json["BALL_SIZE"];
    const short int PADDLE_WIDTH = json["PADDLE_WIDTH"];
    const short int PADDLE_HEIGHT = json["PADDLE_HEIGHT"];

    const short int BIG_FONT_SIZE = json["BIG_FONT_SIZE"];
    const short int NORM_FONT_SIZE = json["NORM_FONT_SIZE"];
    const short int SMALL_FONT_SIZE = json["SMALL_FONT_SIZE"];
    const short int CTRL_SMALL_FONT_SIZE = json["CTRL_SMALL_FONT_SIZE"];

    const short int SFX_VOLUME = json["SFX_VOLUME"];

    //Event booleans
    bool start_screen = true;
    bool run = true;
    bool pause_screen = false;

    //Event variable
    SDL_Event event;

    int ball_x_direction = 1;
    int ball_y_direction = 1;

    //Score counter
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

    //#ifdef _WIN32
    //    FreeConsole();
    //#endif

    MainWindow window("Pong!", WIDTH, HEIGHT);

    TTF_Init();

    //Font
    TTF_Font * font = TTF_OpenFont(FONT_DIR.c_str(), NORM_FONT_SIZE);
    TTF_Font * BigFont = TTF_OpenFont(FONT_DIR.c_str(), BIG_FONT_SIZE);
    TTF_Font * SmallFont = TTF_OpenFont(FONT_DIR.c_str(), SMALL_FONT_SIZE);
    TTF_Font * CtrlSmallFont = TTF_OpenFont(FONT_DIR.c_str(), CTRL_SMALL_FONT_SIZE);

    //Enable Kerning
    TTF_SetFontKerning(font, 1);
    TTF_SetFontKerning(BigFont, 1);
    TTF_SetFontKerning(SmallFont, 1);
    TTF_SetFontKerning(CtrlSmallFont, 1);

    //sfx
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    Mix_Chunk * hit = Mix_LoadWAV(HIT_SFX_DIR.c_str());
    Mix_Chunk * score = Mix_LoadWAV(SCORE_SFX_DIR.c_str());

    if (hit == NULL){
		cout << "SDL2 Mixer error: " << Mix_GetError() << endl;
		return -1;
	}
    Mix_Volume(-1, SFX_VOLUME);

    SDL_Color White = {255, 255, 255, 255};

    //Welcome Text
    //TTF_SizeText() is used to adjust text size according to the font size
    SDL_Texture * WLCM = window.LoadText(BigFont, "Pong!", White);
    SDL_Rect WLCM_rect;
    TTF_SizeText(BigFont, "Pong!", &WLCM_rect.w, &WLCM_rect.h);
    WLCM_rect.x = WIDTH-WLCM_rect.w-(WIDTH-WLCM_rect.w)/2;
    WLCM_rect.y = HEIGHT/3;

    //Control Text
    SDL_Texture * LPC_txt = window.LoadText(CtrlSmallFont, "Paddle 1 Control: W: move up | S: move down", White);
    SDL_Rect LPC_rect;
    TTF_SizeText(CtrlSmallFont, "Paddle 1 Control: W: move up | S: move down", &LPC_rect.w, &LPC_rect.h);
    LPC_rect.x = 5;
    LPC_rect.y = HEIGHT - 3*LPC_rect.h - 5;

    SDL_Texture * RPC_txt = window.LoadText(CtrlSmallFont, "Paddle 2 Control: UP arrow key: move up | DOWN arrow key: move down", White);
    SDL_Rect RPC_rect;
    TTF_SizeText(CtrlSmallFont, "Paddle 2 Control: UP arrow key: move up | DOWN arrow key: move down", &RPC_rect.w, &RPC_rect.h);
    RPC_rect.x = 5;
    RPC_rect.y = HEIGHT - 2*RPC_rect.h - 5;

    SDL_Texture * PAUSE_CTRL_txt = window.LoadText(CtrlSmallFont, "SPACE or P: Pause", White);
    SDL_Rect PAUSE_CTRL_rect;
    TTF_SizeText(CtrlSmallFont, "SPACE or P: Pause", &PAUSE_CTRL_rect.w, &PAUSE_CTRL_rect.h);
    PAUSE_CTRL_rect.x = 5;
    PAUSE_CTRL_rect.y = HEIGHT - PAUSE_CTRL_rect.h - 5;

    //Credit Text
    SDL_Texture * credit = window.LoadText(SmallFont, "@knguyen1401 on GitHub", White);
    SDL_Rect credit_rect;
    TTF_SizeText(SmallFont, "@knguyen1401 on GitHub", &credit_rect.w, &credit_rect.h);
    credit_rect.x = WIDTH - credit_rect.w - 5;
    credit_rect.y = HEIGHT - credit_rect.h - 5;

    //Pause Text
    SDL_Texture * PAUSE = window.LoadText(BigFont, "Paused", White);
    SDL_Rect PAUSE_rect;
    TTF_SizeText(BigFont, "Paused", &PAUSE_rect.w, &PAUSE_rect.h);

    //Control Text
    SDL_Texture * T2 = window.LoadText(font, "Press Space to play!", White);
    SDL_Rect T2_rect;
    TTF_SizeText(font, "Press Space to play!", &T2_rect.w, &T2_rect.h);
    T2_rect.y = HEIGHT/1.5;

    //Pause text 2
    SDL_Texture * T3 = window.LoadText(font, "Press Space to continue", White);
    SDL_Rect T3_rect;
    TTF_SizeText(font, "Press Space to continue", &T3_rect.w, &T3_rect.h);
    T3_rect.y = HEIGHT/1.5;

    //Score 1 Text
    SDL_Texture* ST1 =  window.LoadText(font, y.c_str(), White); //Convert text to surface so that it can be displayed
    SDL_Rect ST1_rect;

    TTF_SizeText(font, y.c_str(), &ST1_rect.w, &ST1_rect.h); //Change the width and height of the text to be the same as the font size
    const int spacing = ST1_rect.w*2/3;
    ST1_rect.x = WIDTH/2-(spacing+ST1_rect.w);
    ST1_rect.y = 5;
    
    //Score 2 Text
    SDL_Texture* ST2 =  window.LoadText(font, z.c_str(), White);
    SDL_Rect ST2_rect;
    TTF_SizeText(font, z.c_str(), &ST2_rect.w, &ST2_rect.h);
    ST2_rect.x = WIDTH/2+spacing;
    
    ST2_rect.y = 5;

    //Ball
    SDL_Texture *ball = window.TextureLoadFromFile(BALL_DIR.c_str());
    SDL_Rect ball_rect = {WIDTH/2-BALL_SIZE/2, HEIGHT/2, BALL_SIZE, BALL_SIZE};

    //Paddle 1
    SDL_Texture *bar1 = window.TextureLoadFromFile(PADDLE_DIR.c_str());
    SDL_Rect bar1_rect = {0, HEIGHT/2-PADDLE_HEIGHT/2, PADDLE_WIDTH, PADDLE_HEIGHT};

    //Paddle 2
    SDL_Texture *bar2 = window.TextureLoadFromFile(PADDLE_DIR.c_str());
    SDL_Rect bar2_rect = {WIDTH-PADDLE_WIDTH, HEIGHT/2-PADDLE_HEIGHT/2, PADDLE_WIDTH, PADDLE_HEIGHT};

    //Separator
    SDL_Texture *Bar3 = window.TextureLoadFromFile(PADDLE_DIR.c_str());
    SDL_Rect bar3_rect;
    bar3_rect.w = 4;
    bar3_rect.h = HEIGHT;

    while(run){
        if (start_screen == true){
            window.ClearRender();
            window.RenderMiddleScreen(WLCM, WIDTH, HEIGHT, WLCM_rect.h, WLCM_rect.w, NULL, WLCM_rect);
            window.RenderMiddleX(T2, WIDTH, T2_rect.w, NULL, T2_rect);
            window.RenderLoad(credit, NULL, &credit_rect);
            window.RenderLoad(LPC_txt, NULL, &LPC_rect);
            window.RenderLoad(RPC_txt, NULL, &RPC_rect);
            window.RenderLoad(PAUSE_CTRL_txt, NULL, &PAUSE_CTRL_rect);
            window.RenderDisplay();
        }
        if (pause_screen == true){
            window.ClearRender(); //clear screen
            window.RenderMiddleScreen(PAUSE, WIDTH, HEIGHT, PAUSE_rect.h, PAUSE_rect.w, NULL, PAUSE_rect);
            window.RenderMiddleX(T3, WIDTH, T3_rect.w, NULL, T3_rect);
            window.RenderLoad(credit, NULL, &credit_rect);
            window.RenderDisplay();
        }
        if (start_screen == false && pause_screen == false){

            ball_rect.x += BALL_SPEED * ball_x_direction;
            ball_rect.y += BALL_SPEED * ball_y_direction;

            if (ball_rect.x <= 0) {
                ball_x_direction = 1;
                score2+=1;

                Mix_PlayChannel(-1, score, 0);

                SDL_DestroyTexture(ST2);
                z = to_string(score2);
                //surface_score1txt = TTF_RenderText_Solid(font, z.c_str(), White); //Re-render the score (or update the score)
                ST2 =  window.LoadText(font, z.c_str(), White);

                TTF_SizeText(font, z.c_str(), &ST2_rect.w, &ST2_rect.h); //Update size

                window.RenderLoad(ST2, NULL, &ST2_rect);
            }

            else if (ball_rect.x + BALL_SIZE >= WIDTH) {
                ball_x_direction = -1;
                score1+=1;

                Mix_PlayChannel(-1, score, 0);

                SDL_DestroyTexture(ST1);
                y  = to_string(score1);
                //surface_score1txt = TTF_RenderText_Solid(font, y.c_str(), White);
                ST1 =  window.LoadText(font, y.c_str(), White);

                TTF_SizeText(font, y.c_str(), &ST1_rect.w, &ST1_rect.h);
                
                if (score1 == 10 || score1 == 100 || score1 == 1000 || score1 == 10000){
                    ST1_rect.x = WIDTH/2-(spacing+ST1_rect.w); //Decreases text "x" position to the left
                }

                window.RenderLoad(ST1, NULL, &ST1_rect);
            }

            //Some physics
            if (ball_rect.y <= 0) {
                ball_y_direction = 1;
            } else if (ball_rect.y + 21 >= HEIGHT) {
                ball_y_direction = -1;
            }

            if (SDL_HasIntersection(&ball_rect, &bar1_rect)){
                ball_x_direction = 1;
                Mix_PlayChannel(-1, hit, 0);
            }
            else if (SDL_HasIntersection(&ball_rect, &bar2_rect)){
                ball_x_direction = -1;
                Mix_PlayChannel(-1, hit, 0);
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
                    bar1_rect.y -= PADDLE_SPEED;
                    if (bar1_rect.y < 0){
                        bar1_rect.y = 0;
                    }
                    break;
                
                case SDLK_s:
                    bar1_rect.y += PADDLE_SPEED;
                    if (bar1_rect.y > HEIGHT-PADDLE_HEIGHT){
                        bar1_rect.y = HEIGHT-PADDLE_HEIGHT;
                    }  
                    break;
                
                case SDLK_UP:
                    bar2_rect.y -= PADDLE_SPEED;
                    if (bar2_rect.y < 0){
                        bar2_rect.y = 0;
                    }
                    break;
                
                case SDLK_DOWN:
                    bar2_rect.y += PADDLE_SPEED;
                    if (bar2_rect.y > HEIGHT-PADDLE_HEIGHT){
                        bar2_rect.y = HEIGHT-PADDLE_HEIGHT;              
                    }
                    break;
                
                case SDLK_p:
                case SDLK_SPACE:
                    if (start_screen == true){
                        window.ClearRender();
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
            window.ClearRender();
            window.RenderLoad(ST1, NULL, &ST1_rect);
            window.RenderLoad(ST2, NULL, &ST2_rect);
            window.RenderLoad(ball, NULL, &ball_rect);
            window.RenderLoad(bar1, NULL, &bar1_rect);
            window.RenderLoad(bar2, NULL, &bar2_rect);
            window.RenderMiddleX(Bar3, WIDTH, bar3_rect.w, NULL, bar3_rect);
            window.RenderDisplay();
        }
    }
    
    //Close Audio
    Mix_FreeChunk(hit);

    //Close fonts
    TTF_CloseFont(font);
    TTF_CloseFont(BigFont);
    TTF_CloseFont(SmallFont);

    //Destroy texts
    SDL_DestroyTexture(WLCM);
    SDL_DestroyTexture(credit);
    SDL_DestroyTexture(ST1);
    SDL_DestroyTexture(ST2);
    SDL_DestroyTexture(T2);
    SDL_DestroyTexture(T3);

    SDL_DestroyTexture(ball);
    SDL_DestroyTexture(bar1);
    SDL_DestroyTexture(bar2);
    SDL_DestroyTexture(Bar3);
    
    window.Cleanup();
    SDL_Quit();
    return 0;
}