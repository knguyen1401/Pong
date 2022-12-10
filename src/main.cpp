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

#ifdef _WIN32
#include <windows.h>
#endif

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

    #ifdef _WIN32
        FreeConsole();
    #endif

    MainWindow window("Pong!", WIDTH, HEIGHT);

    TTF_Init();

    //Font
    TTF_Font * font = TTF_OpenFont(FONT_DIR.c_str(), NORM_FONT_SIZE);
    TTF_Font * BigFont = TTF_OpenFont(FONT_DIR.c_str(), BIG_FONT_SIZE);
    TTF_Font * SmallFont = TTF_OpenFont(FONT_DIR.c_str(), SMALL_FONT_SIZE);
    TTF_Font * ControlSmallFont = TTF_OpenFont(FONT_DIR.c_str(), SMALL_FONT_SIZE);

    //Enable Kerning
    TTF_SetFontKerning(font, 1);
    TTF_SetFontKerning(BigFont, 1);
    TTF_SetFontKerning(SmallFont, 1);

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

    ///Welcome text
    SDL_Surface * welcome = TTF_RenderText_Blended(BigFont, "Pong!", White);
    SDL_Surface * text2 = TTF_RenderText_Blended(font, "Press Space to play!", White);
    SDL_Surface * credit_txt = TTF_RenderText_Blended(SmallFont, "@knguyen1401 on GitHub", White);

    //Control text
    SDL_Surface * LPC = TTF_RenderText_Blended(ControlSmallFont, "Paddle 1 Control: W: move up, S: move down", White); //Left Paddle control text
    SDL_Surface * RPC = TTF_RenderText_Blended(ControlSmallFont, "Paddle 2 Control: Up arrow key: move up, Down arrow key: move down", White); //Right Paddle control text
    SDL_Surface * Pause_ctrl = TTF_RenderText_Blended(ControlSmallFont, "Space: Pause", White);

    //Score
    SDL_Surface * surface_score1txt = TTF_RenderText_Blended(font, y.c_str(), White); //Load text
    SDL_Surface * surface_score2txt = TTF_RenderText_Blended(font, z.c_str(), White);

    //Pause text
    SDL_Surface * paused = TTF_RenderText_Blended(BigFont, "Paused", White);
    SDL_Surface * text3 = TTF_RenderText_Blended(font, "Press Space to Continue", White);

    //Welcome Text
    //TTF_SizeText() is used to adjust text size according to the font size
    SDL_Texture * WLCM = window.TextLoad(welcome);
    SDL_Rect WLCM_loc;
    TTF_SizeText(BigFont, "Pong!", &WLCM_loc.w, &WLCM_loc.h);
    WLCM_loc.x = WIDTH-WLCM_loc.w-(WIDTH-WLCM_loc.w)/2;
    WLCM_loc.y = HEIGHT/3;

    //Control Text
    SDL_Texture * LPC_txt = window.TextLoad(LPC);
    SDL_Rect LPC_loc;
    TTF_SizeText(ControlSmallFont, "Paddle 1 Control: W: move up, S: move down", &LPC_loc.w, &LPC_loc.h);
    LPC_loc.x = 5;
    LPC_loc.y = HEIGHT - 3*LPC_loc.h - 5;

    SDL_Texture * RPC_txt = window.TextLoad(RPC);
    SDL_Rect RPC_loc;
    TTF_SizeText(ControlSmallFont, "Paddle 2 Control: Up arrow key: move up, Down arrow key: move down", &RPC_loc.w, &RPC_loc.h);
    RPC_loc.x = 5;
    RPC_loc.y = HEIGHT - 2*RPC_loc.h - 5;

    SDL_Texture * PAUSE_CTRL_txt = window.TextLoad(Pause_ctrl);
    SDL_Rect PAUSE_CTRL_loc;
    TTF_SizeText(ControlSmallFont, "Space: Pause", &PAUSE_CTRL_loc.w, &PAUSE_CTRL_loc.h);
    PAUSE_CTRL_loc.x = 5;
    PAUSE_CTRL_loc.y = HEIGHT - PAUSE_CTRL_loc.h - 5;

    //Credit Text
    SDL_Texture * credit = window.TextLoad(credit_txt);
    SDL_Rect credit_loc;
    TTF_SizeText(SmallFont, "@knguyen1401 on GitHub", &credit_loc.w, &credit_loc.h);
    credit_loc.x = WIDTH - credit_loc.w - 5;
    credit_loc.y = HEIGHT - credit_loc.h - 5;

    //Pause Text
    SDL_Texture * PAUSE = window.TextLoad(paused);
    SDL_Rect PAUSE_loc;
    TTF_SizeText(BigFont, "Paused", &PAUSE_loc.w, &PAUSE_loc.h);
    PAUSE_loc.x = WIDTH-PAUSE_loc.w-(WIDTH-PAUSE_loc.w)/2;
    PAUSE_loc.y = HEIGHT/3;

    //Control Text
    SDL_Texture * T2 = window.TextLoad(text2);
    SDL_Rect T2_loc;
    TTF_SizeText(font, "Press Space to play!", &T2_loc.w, &T2_loc.h);
    cout << T2_loc.w;
    T2_loc.x = WIDTH-T2_loc.w-(WIDTH-T2_loc.w)/2;
    T2_loc.y = HEIGHT/1.5;

    //Pause text 2
    SDL_Texture * T3 = window.TextLoad(text3);
    SDL_Rect T3_loc;
    TTF_SizeText(font, "Press Space to continue", &T3_loc.w, &T3_loc.h);
    cout << T3_loc.w;
    T3_loc.x = WIDTH-T3_loc.w-(WIDTH-T3_loc.w)/2;
    T3_loc.y = HEIGHT/1.5;

    //Score 1 Text
    SDL_Texture* ST1 =  window.TextLoad(surface_score1txt); //Convert text to surface so that it can be displayed
    SDL_Rect ST1_loc;
    TTF_SizeText(font, y.c_str(), &ST1_loc.w, &ST1_loc.h); //Change the width and height of the text to be the same as the font size
    ST1_loc.x = WIDTH/2-(12+ST1_loc.w);
    ST1_loc.y = 5;
    
    //Score 2 Text
    SDL_Texture* ST2 =  window.TextLoad(surface_score2txt);
    SDL_Rect ST2_loc;
    TTF_SizeText(font, z.c_str(), &ST2_loc.w, &ST2_loc.h);
    ST2_loc.x = WIDTH/2+12;
    ST2_loc.y = 5;

    //Note: Rect = {x, y, w, h}
    //I'm kinda absent-minded sometimes :)
    
    //Ball
    SDL_Texture *ball = window.TextureLoadFromFile(BALL_DIR.c_str());
    SDL_Rect ball_location = {WIDTH/2-BALL_SIZE/2, HEIGHT/2, BALL_SIZE, BALL_SIZE};

    //Paddle 1
    SDL_Texture *bar1 = window.TextureLoadFromFile(PADDLE_DIR.c_str());
    SDL_Rect bar1_location = {0, HEIGHT/2, PADDLE_WIDTH, PADDLE_HEIGHT};

    //Paddle 2
    SDL_Texture *bar2 = window.TextureLoadFromFile(PADDLE_DIR.c_str());
    SDL_Rect bar2_location = {WIDTH-PADDLE_WIDTH, HEIGHT/2, PADDLE_WIDTH, PADDLE_HEIGHT};

    //Separator
    SDL_Texture *Bar3 = window.TextureLoadFromFile(PADDLE_DIR.c_str());
    SDL_Rect Bar3_location = {WIDTH/2-2, 0, 4, HEIGHT};

    while(run){
        if (start_screen == true){
            window.ClearRender();
            window.RenderLoad(WLCM, NULL, &WLCM_loc);
            window.RenderLoad(T2, NULL, &T2_loc);
            window.RenderLoad(credit, NULL, &credit_loc);
            window.RenderLoad(LPC_txt, NULL, &LPC_loc);
            window.RenderLoad(RPC_txt, NULL, &RPC_loc);
            window.RenderLoad(PAUSE_CTRL_txt, NULL, &PAUSE_CTRL_loc);
            window.RenderDisplay();
        }
        if (pause_screen == true){
            window.ClearRender(); //clear screen
            window.RenderLoad(PAUSE, NULL, &PAUSE_loc);
            window.RenderLoad(T3, NULL, &T3_loc);
            window.RenderLoad(credit, NULL, &credit_loc);
            window.RenderDisplay();
        }
        if (start_screen == false && pause_screen == false){

            ball_location.x += BALL_SPEED * ball_x_direction;
            ball_location.y += BALL_SPEED * ball_y_direction;

            if (ball_location.x <= 0) {
                ball_x_direction = 1;
                score2+=1;

                Mix_PlayChannel(-1, score, 0);

                SDL_DestroyTexture(ST2);
                z = to_string(score2);
                surface_score1txt = TTF_RenderText_Solid(font, z.c_str(), White); //Re-render the score (or update the score)
                ST2 =  window.TextLoad(surface_score1txt);

                TTF_SizeText(font, z.c_str(), &ST2_loc.w, &ST2_loc.h); //Update size

                window.RenderLoad(ST2, NULL, &ST2_loc);

            } else if (ball_location.x + BALL_SIZE >= WIDTH) {
                ball_x_direction = -1;
                score1+=1;

                Mix_PlayChannel(-1, score, 0);

                SDL_DestroyTexture(ST1);
                y  = to_string(score1);
                surface_score1txt = TTF_RenderText_Solid(font, y.c_str(), White);
                ST1 =  window.TextLoad(surface_score1txt);

                TTF_SizeText(font, y.c_str(), &ST1_loc.w, &ST1_loc.h);
                
                if (score1 == 10 || score1 == 100 || score1 == 1000 || score1 == 10000){
                    ST1_loc.x = WIDTH/2-(12+ST1_loc.w); //Decreases text "x" position to the left
                }

                window.RenderLoad(ST1, NULL, &ST1_loc);
            }

            //Some physics
            if (ball_location.y <= 0) {
                ball_y_direction = 1;
            } else if (ball_location.y + 21 >= HEIGHT) {
                ball_y_direction = -1;
            }

            if (SDL_HasIntersection(&ball_location, &bar1_location)){
                ball_x_direction = 1;
                Mix_PlayChannel(-1, hit, 0);
            }
            else if (SDL_HasIntersection(&ball_location, &bar2_location)){
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
            window.RenderLoad(ST1, NULL, &ST1_loc);
            window.RenderLoad(ST2, NULL, &ST2_loc);
            window.RenderLoad(ball, NULL, &ball_location);
            window.RenderLoad(bar1, NULL, &bar1_location);
            window.RenderLoad(bar2, NULL, &bar2_location);
            window.RenderLoad(Bar3, NULL, &Bar3_location);
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
