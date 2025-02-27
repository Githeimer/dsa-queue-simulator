#include "road.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900
#define ROAD_WIDTH 180 // Each road's total width
#define LANE_WIDTH 60  // Width of each lane

// Vehicle structure
typedef struct Vehicle
{
    float x, y;        // Position of the vehicle
    float speed;       // Vehicle's speed in pixels per frame
    int width, height; // Vehicle dimensions
} Vehicle;

// Function to initialize vehicle
void initVehicle(Vehicle *vehicle, float startX, float startY, float speed, int width, int height)
{
    vehicle->x = startX;
    vehicle->y = startY;
    vehicle->speed = speed;
    vehicle->width = width;
    vehicle->height = height;
}

// Function to move vehicle based on its position and direction
void moveVehicle(Vehicle *vehicle)
{
    vehicle->x += vehicle->speed; // Move the vehicle in the x-direction
    if (vehicle->x > WINDOW_WIDTH)
    {
        vehicle->x = -vehicle->width; // Wrap the vehicle back to the left side
    }
}

// Function to draw vehicle
void drawVehicle(SDL_Renderer *renderer, Vehicle *vehicle)
{
    SDL_Rect vehicleRect = {(int)vehicle->x, (int)vehicle->y, vehicle->width, vehicle->height};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for the vehicle
    SDL_RenderFillRect(renderer, &vehicleRect);
}

// Function to render and animate vehicle based on entry and exit lanes
void renderVehicle(SDL_Renderer *renderer, const char *entryLane, const char *exitLane)
{
    Vehicle vehicle;
    float startX = 0, startY = 0;

    // Set initial position based on entry lane
    if (strcmp(entryLane, "AL2") == 0)
    {
        startX = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + LANE_WIDTH;  // L2 of road A
        startY = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + LANE_WIDTH; // Middle vertical lane
    }
    else if (strcmp(entryLane, "DL2") == 0)
    {
        startX = 0;                                               // L2 of road D (Left road)
        startY = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + LANE_WIDTH; // Middle horizontal lane
    }

    initVehicle(&vehicle, startX, startY, 2.0f, 40, 20); // Speed 2 pixels/frame, dimensions 40x20

    // Start rendering and moving the vehicle
    while (vehicle.x < WINDOW_WIDTH)
    {
        SDL_RenderClear(renderer); // Clear the screen

        DrawRoad(renderer);              // Draw the road
        moveVehicle(&vehicle);           // Move the vehicle
        drawVehicle(renderer, &vehicle); // Draw the moving vehicle

        SDL_RenderPresent(renderer); // Present the renderer
        SDL_Delay(16);               // Delay for 60 FPS
    }
}
