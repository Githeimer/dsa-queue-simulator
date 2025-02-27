#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL3/SDL.h>

typedef struct AnimationVehicle
{
    int id;
    const char *entryLane;
    const char *exitLane;
    float x, y;    // Vehicle's position
    SDL_Rect rect; // SDL rectangle for rendering
} AnimationVehicle;

void RenderVehicle(SDL_Renderer *renderer, const char *entryLane, const char *exitLane);
#endif
