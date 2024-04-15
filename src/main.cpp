#include <iostream>

#define SPDLOG_USE_STD_FORMART
#include <spdlog/spdlog.h>

#include <SDL2/SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;

int main(int argc, char** argv)
{
    SDL_Surface* screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
       spdlog::error("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
    }
    else {
        //Create window
        window = SDL_CreateWindow("minty", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            spdlog::error("Window could not be created! SDL_Error: {}\n", SDL_GetError());
            return -1;
        }

        //Get window surface
        screenSurface = SDL_GetWindowSurface(window);

        //Fill the surface white ish
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFA, 0xFA, 0xFA));

        //Update the surface
        SDL_UpdateWindowSurface(window);

        //Hack to get window to stay up
        SDL_Event e; 
        bool quit = false; 
        while (quit == false)
        { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }
    }

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();
    return 0;
}