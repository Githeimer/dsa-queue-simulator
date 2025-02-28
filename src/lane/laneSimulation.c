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
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 160); // Forest green for grass
    SDL_FRect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &background);

    for (int i = 0; i < 200; i++)
    {
        int x = rand() % WINDOW_WIDTH;
        int y = rand() % WINDOW_HEIGHT;

        // Skip areas where roads and sidewalks will be
        if ((x > WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - 20 && x < WINDOW_WIDTH / 2 + ROAD_WIDTH / 2 + 20) ||
            (y > WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - 20 && y < WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2 + 20))
            continue;

        SDL_FRect grassTuft = {x, y, 4, 4};
        SDL_RenderFillRect(renderer, &grassTuft);
    }

    // Define sidewalk width
    int SIDEWALK_WIDTH = 15;

    // Draw sidewalks (light gray)
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    // Vertical sidewalks (beside A and C roads)
    SDL_FRect sidewalkA1 = {WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH, 0, SIDEWALK_WIDTH, WINDOW_HEIGHT / 2};
    SDL_FRect sidewalkA2 = {WINDOW_WIDTH / 2 + ROAD_WIDTH / 2, 0, SIDEWALK_WIDTH, WINDOW_HEIGHT / 2};
    SDL_FRect sidewalkC1 = {WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH, WINDOW_HEIGHT / 2, SIDEWALK_WIDTH, WINDOW_HEIGHT / 2};
    SDL_FRect sidewalkC2 = {WINDOW_WIDTH / 2 + ROAD_WIDTH / 2, WINDOW_HEIGHT / 2, SIDEWALK_WIDTH, WINDOW_HEIGHT / 2};

    // Horizontal sidewalks (beside B and D roads)
    SDL_FRect sidewalkB1 = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH, WINDOW_WIDTH / 2, SIDEWALK_WIDTH};
    SDL_FRect sidewalkB2 = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2, WINDOW_WIDTH / 2, SIDEWALK_WIDTH};
    SDL_FRect sidewalkD1 = {0, WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH, WINDOW_WIDTH / 2, SIDEWALK_WIDTH};
    SDL_FRect sidewalkD2 = {0, WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2, WINDOW_WIDTH / 2, SIDEWALK_WIDTH};

    // Draw sidewalks
    SDL_RenderFillRect(renderer, &sidewalkA1);
    SDL_RenderFillRect(renderer, &sidewalkA2);
    SDL_RenderFillRect(renderer, &sidewalkC1);
    SDL_RenderFillRect(renderer, &sidewalkC2);
    SDL_RenderFillRect(renderer, &sidewalkB1);
    SDL_RenderFillRect(renderer, &sidewalkB2);
    SDL_RenderFillRect(renderer, &sidewalkD1);
    SDL_RenderFillRect(renderer, &sidewalkD2);

    // Add sidewalk texture (small grid pattern)
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

    // Vertical sidewalk grid lines
    for (int x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH; x <= WINDOW_WIDTH / 2 + ROAD_WIDTH / 2 + SIDEWALK_WIDTH; x += 5)
    {
        if (x >= WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 && x <= WINDOW_WIDTH / 2 + ROAD_WIDTH / 2)
            continue; // Skip the road area

        SDL_RenderLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }

    // Horizontal sidewalk grid lines
    for (int y = 0; y < WINDOW_HEIGHT; y += 5)
    {
        // Left side of vertical road
        SDL_RenderLine(renderer,
                       WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH,
                       y,
                       WINDOW_WIDTH / 2 - ROAD_WIDTH / 2,
                       y);

        // Right side of vertical road
        SDL_RenderLine(renderer,
                       WINDOW_WIDTH / 2 + ROAD_WIDTH / 2,
                       y,
                       WINDOW_WIDTH / 2 + ROAD_WIDTH / 2 + SIDEWALK_WIDTH,
                       y);
    }

    // Horizontal sidewalk grid lines
    for (int y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH; y <= WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2 + SIDEWALK_WIDTH; y += 5)
    {
        if (y >= WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 && y <= WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2)
            continue; // Skip the road area

        SDL_RenderLine(renderer, 0, y, WINDOW_WIDTH, y);
    }

    // Vertical sidewalk grid lines
    for (int x = 0; x < WINDOW_WIDTH; x += 5)
    {
        // Top side of horizontal road
        SDL_RenderLine(renderer,
                       x,
                       WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH,
                       x,
                       WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2);

        // Bottom side of horizontal road
        SDL_RenderLine(renderer,
                       x,
                       WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2,
                       x,
                       WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2 + SIDEWALK_WIDTH);
    }

    // Draw road edges (black border/curb)
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

    // Vertical road edges
    SDL_FRect roadABorder1 = {WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - 2, 0, 2, WINDOW_HEIGHT / 2};
    SDL_FRect roadABorder2 = {WINDOW_WIDTH / 2 + ROAD_WIDTH / 2, 0, 2, WINDOW_HEIGHT / 2};
    SDL_FRect roadCBorder1 = {WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - 2, WINDOW_HEIGHT / 2, 2, WINDOW_HEIGHT / 2};
    SDL_FRect roadCBorder2 = {WINDOW_WIDTH / 2 + ROAD_WIDTH / 2, WINDOW_HEIGHT / 2, 2, WINDOW_HEIGHT / 2};

    // Horizontal road edges
    SDL_FRect roadBBorder1 = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - 2, WINDOW_WIDTH / 2, 2};
    SDL_FRect roadBBorder2 = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2, WINDOW_WIDTH / 2, 2};
    SDL_FRect roadDBorder1 = {0, WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - 2, WINDOW_WIDTH / 2, 2};
    SDL_FRect roadDBorder2 = {0, WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2, WINDOW_WIDTH / 2, 2};

    SDL_RenderFillRect(renderer, &roadABorder1);
    SDL_RenderFillRect(renderer, &roadABorder2);
    SDL_RenderFillRect(renderer, &roadCBorder1);
    SDL_RenderFillRect(renderer, &roadCBorder2);
    SDL_RenderFillRect(renderer, &roadBBorder1);
    SDL_RenderFillRect(renderer, &roadBBorder2);
    SDL_RenderFillRect(renderer, &roadDBorder1);
    SDL_RenderFillRect(renderer, &roadDBorder2);

    // Gray road with texture (dark asphalt)
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);

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

    // Add road texture/imperfections
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 100);
    for (int i = 0; i < 300; i++)
    {
        int x, y;
        // Choose random points along the roads
        if (rand() % 2)
        {
            // Vertical roads
            x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + rand() % ROAD_WIDTH;
            y = rand() % WINDOW_HEIGHT;
        }
        else
        {
            // Horizontal roads
            x = rand() % WINDOW_WIDTH;
            y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + rand() % ROAD_WIDTH;
        }
        SDL_FRect roadSpeck = {x, y, 2 + rand() % 3, 2 + rand() % 3};
        SDL_RenderFillRect(renderer, &roadSpeck);
    }

    // Draw intersection with slightly different shade
    SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
    SDL_FRect intersection = {
        WINDOW_WIDTH / 2 - ROAD_WIDTH / 2,
        WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2,
        ROAD_WIDTH,
        ROAD_WIDTH};
    SDL_RenderFillRect(renderer, &intersection);

    // Draw lane dividers (Yellow lines for lanes)
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // Gold yellow for better visibility

    int dashLength = 15, gapLength = 15;

    // Vertical lane dividers (A & C) - Now drawing two lines for three lanes
    for (int i = 1; i <= 2; i++)
    {
        int x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH;
        for (int y = 0; y < WINDOW_HEIGHT; y += dashLength + gapLength)
        {
            // Skip intersection area
            if (y > WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 && y < WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2)
                continue;

            SDL_RenderLine(renderer, x, y, x, y + dashLength);
        }
    }

    // Horizontal lane dividers (B & D) - Now drawing two lines for three lanes
    for (int i = 1; i <= 2; i++)
    {
        int y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + i * LANE_WIDTH;
        for (int x = 0; x < WINDOW_WIDTH; x += dashLength + gapLength)
        {
            // Skip intersection area
            if (x > WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 && x < WINDOW_WIDTH / 2 + ROAD_WIDTH / 2)
                continue;

            SDL_RenderLine(renderer, x, y, x + dashLength, y);
        }
    }

    // Add crosswalks at the intersection edges
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int crosswalkWidth = 5;
    int crosswalkGap = 5;

    // Top crosswalk
    for (int x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + crosswalkGap;
         x < WINDOW_WIDTH / 2 + ROAD_WIDTH / 2;
         x += crosswalkWidth + crosswalkGap)
    {
        SDL_FRect stripe = {x, WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - 10, crosswalkWidth, 20};
        SDL_RenderFillRect(renderer, &stripe);
    }

    // Bottom crosswalk
    for (int x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + crosswalkGap;
         x < WINDOW_WIDTH / 2 + ROAD_WIDTH / 2;
         x += crosswalkWidth + crosswalkGap)
    {
        SDL_FRect stripe = {x, WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2 - 10, crosswalkWidth, 20};
        SDL_RenderFillRect(renderer, &stripe);
    }

    // Left crosswalk
    for (int y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + crosswalkGap;
         y < WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2;
         y += crosswalkWidth + crosswalkGap)
    {
        SDL_FRect stripe = {WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - 10, y, 20, crosswalkWidth};
        SDL_RenderFillRect(renderer, &stripe);
    }

    // Right crosswalk
    for (int y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + crosswalkGap;
         y < WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2;
         y += crosswalkWidth + crosswalkGap)
    {
        SDL_FRect stripe = {WINDOW_WIDTH / 2 + ROAD_WIDTH / 2 - 10, y, 20, crosswalkWidth};
        SDL_RenderFillRect(renderer, &stripe);
    }

    // Add fire hydrants at some corners
    if (rand() % 2 == 0)
    {
        // Draw a fire hydrant at the top-left corner
        SDL_SetRenderDrawColor(renderer, 220, 60, 50, 255); // Red
        SDL_FRect hydrantBase = {WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH + 5,
                                 WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH + 5,
                                 5, 8};
        SDL_RenderFillRect(renderer, &hydrantBase);
        SDL_FRect hydrantTop = {WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH + 4,
                                WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 - SIDEWALK_WIDTH + 3,
                                7, 4};
        SDL_RenderFillRect(renderer, &hydrantTop);
    }

    if (rand() % 2 == 0)
    {
        // Draw a fire hydrant at the bottom-right corner
        SDL_SetRenderDrawColor(renderer, 220, 60, 50, 255); // Red
        SDL_FRect hydrantBase = {WINDOW_WIDTH / 2 + ROAD_WIDTH / 2 + SIDEWALK_WIDTH - 10,
                                 WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2 + SIDEWALK_WIDTH - 13,
                                 5, 8};
        SDL_RenderFillRect(renderer, &hydrantBase);
        SDL_FRect hydrantTop = {WINDOW_WIDTH / 2 + ROAD_WIDTH / 2 + SIDEWALK_WIDTH - 11,
                                WINDOW_HEIGHT / 2 + ROAD_WIDTH / 2 + SIDEWALK_WIDTH - 15,
                                7, 4};
        SDL_RenderFillRect(renderer, &hydrantTop);
    }

    // Optionally draw lane labels for debugging
    // DrawLaneLabels(renderer);
}
