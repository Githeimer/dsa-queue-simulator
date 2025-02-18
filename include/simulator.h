// simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <SDL3/SDL.h>
#include "./queue.h">

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define NUM_LANES 4

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Queue *lanes[NUM_LANES]; // Array of queues for each lane
    bool running;
} Simulator;

// Simulator functions
bool initSimulator(Simulator *sim);
void updateSimulator(Simulator *sim);
void renderSimulator(Simulator *sim);
void cleanupSimulator(Simulator *sim);

#endif