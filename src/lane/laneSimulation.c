#include "road.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900
#define ROAD_WIDTH 180  // Each road's total width (3 lanes x 60 width)
#define LANE_WIDTH 60   // Width of each lane
#define VEHICLE_SIZE 30 // Vehicle size (width and height)

// Debug function to visualize lane numbers
void DrawLaneLabels(SDL_Renderer *renderer)
{
    // Create a font-like rendering for lane labels
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Road A labels (top vertical road)
    for (int i = 0; i < 3; i++)
    {
        char label[3] = {'A', 'L', '1' + i};
        int x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH + LANE_WIDTH / 2 - 10;
        int y = 20;

        // Draw a small black box to represent text
        SDL_FRect textRect = {x, y, 20, 15};
        SDL_RenderFillRect(renderer, &textRect);
    }

    // Road B labels (right horizontal road)
    for (int i = 0; i < 3; i++)
    {
        int x = WINDOW_WIDTH - 40;
        int y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH + LANE_WIDTH / 2 - 10;

        SDL_FRect textRect = {x, y, 20, 15};
        SDL_RenderFillRect(renderer, &textRect);
    }

    // Road C labels (bottom vertical road)
    for (int i = 0; i < 3; i++)
    {
        int x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH + LANE_WIDTH / 2 - 10;
        int y = WINDOW_HEIGHT - 40;

        SDL_FRect textRect = {x, y, 20, 15};
        SDL_RenderFillRect(renderer, &textRect);
    }

    // Road D labels (left horizontal road)
    for (int i = 0; i < 3; i++)
    {
        int x = 20;
        int y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH + LANE_WIDTH / 2 - 10;

        SDL_FRect textRect = {x, y, 20, 15};
        SDL_RenderFillRect(renderer, &textRect);
    }
}

void DrawRoad(SDL_Renderer *renderer)
{
    // Set background color (green for grass/terrain)
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 60);
    SDL_FRect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &background);

    // Gray color for roads
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);

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
    SDL_SetRenderDrawColor(renderer, 255, 250, 0, 255);

    int dashLength = 15, gapLength = 15;

    // Vertical lane dividers (A & C) - Now drawing two lines for three lanes
    for (int i = 1; i <= 2; i++)
    {
        int x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH;
        for (int y = 0; y < WINDOW_HEIGHT; y += dashLength + gapLength)
        {
            SDL_RenderLine(renderer, x, y, x, y + dashLength);
        }
    }

    // Horizontal lane dividers (B & D) - Now drawing two lines for three lanes
    for (int i = 1; i <= 2; i++)
    {
        int y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH;
        for (int x = 0; x < WINDOW_WIDTH; x += dashLength + gapLength)
        {
            SDL_RenderLine(renderer, x, y, x + dashLength, y);
        }
    }

    // Optionally draw lane labels for debugging
    // DrawLaneLabels(renderer);
}
