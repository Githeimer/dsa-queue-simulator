#include "road.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900
#define ROAD_WIDTH 180  // Each road's total width
#define LANE_WIDTH 60   // Width of each lane
#define VEHICLE_SIZE 30 // Vehicle size (width and height)

void DrawRoad(SDL_Renderer *renderer)
{
    // Gray color for roads
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    // Vertical roads (A - Top, C - Bottom)
    SDL_FRect roadA = {WINDOW_WIDTH / 2 - ROAD_WIDTH / 2, 0, ROAD_WIDTH, WINDOW_HEIGHT / 2};
    SDL_FRect roadC = {WINDOW_WIDTH / 2 - ROAD_WIDTH / 2, WINDOW_HEIGHT / 2, ROAD_WIDTH, WINDOW_HEIGHT / 2};

    // Horizontal roads (B - Right, D - Left)
    SDL_FRect roadB = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2, WINDOW_WIDTH / 2, ROAD_WIDTH};
    SDL_FRect roadD = {0, WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2, WINDOW_WIDTH / 2, ROAD_WIDTH};

    // Draw roads
    SDL_RenderFillRect(renderer, &roadA);
    SDL_RenderFillRect(renderer, &roadB);
    SDL_RenderFillRect(renderer, &roadC);
    SDL_RenderFillRect(renderer, &roadD);

    // Draw lane dividers (White lines for lanes)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int dashLength = 15, gapLength = 15;

    // Vertical lane dividers (A & C)
    for (int i = 1; i <= 2; i++)
    {
        int x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH;
        for (int y = 0; y < WINDOW_HEIGHT; y += dashLength + gapLength)
        {
            SDL_RenderLine(renderer, x, y, x, y + dashLength);
        }
    }

    // Horizontal lane dividers (B & D)
    for (int i = 1; i <= 2; i++)
    {
        int y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH;
        for (int x = 0; x < WINDOW_WIDTH; x += dashLength + gapLength)
        {
            SDL_RenderLine(renderer, x, y, x + dashLength, y);
        }
    }
}

// Function to render the vehicle on the road at the correct lane and position with animation
void RenderVehicle(SDL_Renderer *renderer, const char *entryLane, const char *exitLane)
{
    // Vehicle color (Red for simplicity)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Initial position of the vehicle
    float vehiclePosition = 0.0f;
    bool isExiting = false;

    // Starting position based on entry lane
    int x = 0, y = 0;
    int entryLaneIndex = entryLane[1] - '1'; // L1 -> 0, L2 -> 1
    int exitLaneIndex = exitLane[1] - '1';

    // Determine the lane's position (A, B, C, D)
    if (entryLane[0] == 'A') // Road A (Vertical - Top)
    {
        x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + entryLaneIndex * LANE_WIDTH + LANE_WIDTH / 2;
        y = -VEHICLE_SIZE; // Start above the screen to enter from top
    }
    else if (entryLane[0] == 'C') // Road C (Vertical - Bottom)
    {
        x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + entryLaneIndex * LANE_WIDTH + LANE_WIDTH / 2;
        y = WINDOW_HEIGHT + VEHICLE_SIZE; // Start below the screen to enter from bottom
    }
    else if (entryLane[0] == 'B') // Road B (Horizontal - Right)
    {
        x = WINDOW_WIDTH + VEHICLE_SIZE; // Start from the right side of the screen
        y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + entryLaneIndex * LANE_WIDTH + LANE_WIDTH / 2;
    }
    else if (entryLane[0] == 'D') // Road D (Horizontal - Left)
    {
        x = -VEHICLE_SIZE; // Start from the left side of the screen
        y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + entryLaneIndex * LANE_WIDTH + LANE_WIDTH / 2;
    }

    // Move the vehicle along the correct road
    if (entryLane[0] == 'A' || entryLane[0] == 'C') // Vertical movement (Road A or C)
    {
        float targetY = (entryLane[0] == 'A') ? WINDOW_HEIGHT / 2 : 0;
        while (y != targetY)
        {
            y += (entryLane[0] == 'A') ? 1 : -1;
            SDL_RenderClear(renderer);
            DrawRoad(renderer);
            SDL_Rect vehicleRect = {x, y, VEHICLE_SIZE, VEHICLE_SIZE};
            SDL_RenderFillRect(renderer, &vehicleRect);
            SDL_RenderPresent(renderer);
            SDL_Delay(10); // Delay for animation effect
        }
    }
    else if (entryLane[0] == 'B' || entryLane[0] == 'D') // Horizontal movement (Road B or D)
    {
        float targetX = (entryLane[0] == 'B') ? WINDOW_WIDTH : 0;
        while (x != targetX)
        {
            x += (entryLane[0] == 'B') ? -1 : 1;
            SDL_RenderClear(renderer);
            DrawRoad(renderer);
            SDL_Rect vehicleRect = {x, y, VEHICLE_SIZE, VEHICLE_SIZE};
            SDL_RenderFillRect(renderer, &vehicleRect);
            SDL_RenderPresent(renderer);
            SDL_Delay(10); // Delay for animation effect
        }
    }

    // After the vehicle reaches the entry position, move towards exit
    if (exitLane[0] == 'A' || exitLane[0] == 'C') // Vertical exit
    {
        while (!isExiting)
        {
            if (exitLane[0] == 'A' && y > WINDOW_HEIGHT / 2)
            {
                y -= 1;
            }
            else if (exitLane[0] == 'C' && y < WINDOW_HEIGHT / 2)
            {
                y += 1;
            }
            else
            {
                isExiting = true;
            }

            SDL_RenderClear(renderer);
            DrawRoad(renderer);
            SDL_Rect vehicleRect = {x, y, VEHICLE_SIZE, VEHICLE_SIZE};
            SDL_RenderFillRect(renderer, &vehicleRect);
            SDL_RenderPresent(renderer);
            SDL_Delay(10);
        }
    }
    else if (exitLane[0] == 'B' || exitLane[0] == 'D') // Horizontal exit
    {
        while (!isExiting)
        {
            if (exitLane[0] == 'B' && x < WINDOW_WIDTH)
            {
                x += 1;
            }
            else if (exitLane[0] == 'D' && x > 0)
            {
                x -= 1;
            }
            else
            {
                isExiting = true;
            }

            SDL_RenderClear(renderer);
            DrawRoad(renderer);
            SDL_Rect vehicleRect = {x, y, VEHICLE_SIZE, VEHICLE_SIZE};
            SDL_RenderFillRect(renderer, &vehicleRect);
            SDL_RenderPresent(renderer);
            SDL_Delay(10);
        }
    }
}
