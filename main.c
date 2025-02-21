#include <SDL3/SDL.h>
#include <stdio.h>
#include "traffic_simulator.h" // Include the simulator header

int main(int argc, char *argv[])
{
    // Initialize SDL3
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Start SDL3 Traffic Simulation
    runTrafficSimulation();

    // Clean up SDL before exiting
    SDL_Quit();

    return 0;
}
