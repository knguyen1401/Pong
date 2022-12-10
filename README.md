# Pong

## Warning: Stupid code

### Library used: 
SDL2, SDL2 Image, SDL2 TTF, and nlohmann JSON.

### SFX:
Chrome dino SFX

### Compile:
On Windows (static compilation) :\
`g++ -std=c++20 src\*.cpp icon.o -m64 -s -IC:\msys64\mingw64\include -L"C:\msys64\mingw64\lib" -Wall -Wextra -pedantic -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf  -lmingw32 -mwindows -lSDL2main -lSDL2  -lm -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8 -lusp10 -lgdi32 -lrpcrt4 -static -o bin\main`\
Note: Create the "bin" folder in the current working directory and after the compilation, move the "res" folder to the "bin" folder.

### Edit settings:
There's a json file for that. Go to "res/settings.json" and then modify the settings.
