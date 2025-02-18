// src/simulator/simulator.c
#include <stdio.h>
#include <SDL3/SDL.h>
#include "../queue.h"
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    printf("Simulator initialized\n");

    SDL_Quit();
    return 0;
}