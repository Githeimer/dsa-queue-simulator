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
    SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255);
    SDL_FRect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &background);

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

// Function to render the vehicle on the road at the correct lane and position with animation
void RenderVehicle(SDL_Renderer *renderer, const char *entryLane, const char *exitLane)
{
    printf("\nVehicle Simulation: Entry %s, Exit %s\n", entryLane, exitLane);

    // Validate that the lane format is correct (e.g., "AL1")
    if (strlen(entryLane) < 3 || strlen(exitLane) < 3)
    {
        printf("Invalid lane format. Expected format: 'XLY' where X is road and Y is lane number.\n");
        return;
    }

    // Extract the road (A, B, C, D) and lane number (1, 2, 3)
    char entryRoad = entryLane[0];
    int entryLaneNum = entryLane[2] - '0'; // Convert character to number

    char exitRoad = exitLane[0];
    int exitLaneNum = exitLane[2] - '0';

    // Validate lane numbers (should be 1, 2, or 3)
    if (entryLaneNum < 1 || entryLaneNum > 3 || exitLaneNum < 1 || exitLaneNum > 3)
    {
        printf("Invalid lane number. Lane numbers should be 1, 2, or 3.\n");
        return;
    }

    // The lane index is 0-based (lane 1 = index 0, lane 2 = index 1, lane 3 = index 2)
    int entryLaneIndex = entryLaneNum - 1;
    int exitLaneIndex = exitLaneNum - 1;

    // Starting position based on entry lane
    float x = 0.0f, y = 0.0f;
    float dx = 0.0f, dy = 0.0f; // Movement direction

    // Calculate center of lane based on lane index (0, 1, or 2)
    float laneCenterOffset = LANE_WIDTH / 2 + entryLaneIndex * LANE_WIDTH;

    // Determine the initial position and movement direction based on entry road
    if (entryRoad == 'A') // Road A (Vertical - Top)
    {
        x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + laneCenterOffset - VEHICLE_SIZE / 2;
        y = -VEHICLE_SIZE; // Start above the screen
        dy = 3.0f;         // Move downward
    }
    else if (entryRoad == 'C') // Road C (Vertical - Bottom)
    {
        x = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + laneCenterOffset - VEHICLE_SIZE / 2;
        y = WINDOW_HEIGHT + VEHICLE_SIZE; // Start below the screen
        dy = -3.0f;                       // Move upward
    }
    else if (entryRoad == 'B') // Road B (Horizontal - Right)
    {
        x = WINDOW_WIDTH + VEHICLE_SIZE; // Start off-screen to the right
        y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + laneCenterOffset - VEHICLE_SIZE / 2;
        dx = -3.0f; // Move leftward
    }
    else if (entryRoad == 'D') // Road D (Horizontal - Left)
    {
        x = -VEHICLE_SIZE; // Start off-screen to the left
        y = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + laneCenterOffset - VEHICLE_SIZE / 2;
        dx = 3.0f; // Move rightward
    }
    else
    {
        printf("Invalid entry road: %c\n", entryRoad);
        return;
    }

    // Main animation loop
    bool isMovingToIntersection = true;
    bool hasReachedDestination = false;

    // Determine the intersection point (center of the window)
    float intersectionX = WINDOW_WIDTH / 2 - VEHICLE_SIZE / 2;
    float intersectionY = WINDOW_HEIGHT / 2 - VEHICLE_SIZE / 2;

    // Set up exit lane position
    float exitLaneCenterOffset = LANE_WIDTH / 2 + exitLaneIndex * LANE_WIDTH;
    float targetX = 0, targetY = 0;

    if (exitRoad == 'A')
    {
        targetX = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
        targetY = -VEHICLE_SIZE; // Off screen at the top
    }
    else if (exitRoad == 'C')
    {
        targetX = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
        targetY = WINDOW_HEIGHT + VEHICLE_SIZE; // Off screen at the bottom
    }
    else if (exitRoad == 'B')
    {
        targetX = WINDOW_WIDTH + VEHICLE_SIZE; // Off screen at the right
        targetY = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
    }
    else if (exitRoad == 'D')
    {
        targetX = -VEHICLE_SIZE; // Off screen at the left
        targetY = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
    }
    else
    {
        printf("Invalid exit road: %c\n", exitRoad);
        return;
    }

    // Animation loop
    while (!hasReachedDestination)
    {
        // Handle SDL events to prevent the application from freezing
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                return; // Exit the function if the window is closed
            }
        }

        // Update vehicle position
        x += dx;
        y += dy;

        // Calculate distance to intersection
        float distToIntersectionX = (dx > 0) ? intersectionX - x : x - intersectionX;
        float distToIntersectionY = (dy > 0) ? intersectionY - y : y - intersectionY;

        // Check if vehicle has reached the intersection
        bool reachedIntersectionX = (dx > 0 && x >= intersectionX) ||
                                    (dx < 0 && x <= intersectionX) ||
                                    dx == 0 || distToIntersectionX < 1.0f;
        bool reachedIntersectionY = (dy > 0 && y >= intersectionY) ||
                                    (dy < 0 && y <= intersectionY) ||
                                    dy == 0 || distToIntersectionY < 1.0f;

        // If we've reached the intersection, switch direction toward exit
        if (isMovingToIntersection && reachedIntersectionX && reachedIntersectionY)
        {
            isMovingToIntersection = false;

            // Reset position to exactly the intersection to avoid drift
            x = intersectionX;
            y = intersectionY;

            // Set new direction towards exit
            dx = 0;
            dy = 0;

            if (exitRoad == 'A')
            {
                // First move horizontally to align with the exit lane
                float exitX = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
                if (x < exitX)
                {
                    dx = 3.0f; // Move right to reach exit lane
                }
                else if (x > exitX)
                {
                    dx = -3.0f; // Move left to reach exit lane
                }
                else
                {
                    dy = -3.0f; // Already aligned, move upward
                }
            }
            else if (exitRoad == 'C')
            {
                // First move horizontally to align with the exit lane
                float exitX = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
                if (x < exitX)
                {
                    dx = 3.0f; // Move right to reach exit lane
                }
                else if (x > exitX)
                {
                    dx = -3.0f; // Move left to reach exit lane
                }
                else
                {
                    dy = 3.0f; // Already aligned, move downward
                }
            }
            else if (exitRoad == 'B')
            {
                // First move vertically to align with the exit lane
                float exitY = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
                if (y < exitY)
                {
                    dy = 3.0f; // Move down to reach exit lane
                }
                else if (y > exitY)
                {
                    dy = -3.0f; // Move up to reach exit lane
                }
                else
                {
                    dx = 3.0f; // Already aligned, move rightward
                }
            }
            else if (exitRoad == 'D')
            {
                // First move vertically to align with the exit lane
                float exitY = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
                if (y < exitY)
                {
                    dy = 3.0f; // Move down to reach exit lane
                }
                else if (y > exitY)
                {
                    dy = -3.0f; // Move up to reach exit lane
                }
                else
                {
                    dx = -3.0f; // Already aligned, move leftward
                }
            }
        }

        // If we've aligned with the exit lane (after initial intersection positioning)
        if (!isMovingToIntersection && ((dx != 0 && dy == 0) || (dx == 0 && dy != 0)))
        {
            // For vertical exits (A, C)
            if ((exitRoad == 'A' || exitRoad == 'C') && dx != 0)
            {
                float exitX = WINDOW_WIDTH / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
                if ((dx > 0 && x >= exitX) || (dx < 0 && x <= exitX))
                {
                    // We've reached the horizontal alignment, now move vertically
                    x = exitX; // Snap to exact position
                    dx = 0;
                    dy = (exitRoad == 'A') ? -3.0f : 3.0f;
                }
            }
            // For horizontal exits (B, D)
            else if ((exitRoad == 'B' || exitRoad == 'D') && dy != 0)
            {
                float exitY = WINDOW_HEIGHT / 2 - ROAD_WIDTH / 2 + exitLaneCenterOffset - VEHICLE_SIZE / 2;
                if ((dy > 0 && y >= exitY) || (dy < 0 && y <= exitY))
                {
                    // We've reached the vertical alignment, now move horizontally
                    y = exitY; // Snap to exact position
                    dy = 0;
                    dx = (exitRoad == 'B') ? 3.0f : -3.0f;
                }
            }
        }

        // Check if vehicle has reached the destination (off-screen in exit direction)
        if (!isMovingToIntersection)
        {
            if ((exitRoad == 'A' && y < -VEHICLE_SIZE) ||
                (exitRoad == 'C' && y > WINDOW_HEIGHT) ||
                (exitRoad == 'B' && x > WINDOW_WIDTH) ||
                (exitRoad == 'D' && x < -VEHICLE_SIZE))
            {
                hasReachedDestination = true;
            }
        }

        // Render the scene
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the road
        DrawRoad(renderer);

        // Draw the vehicle (use different colors for different roads for debugging)
        if (isMovingToIntersection)
        {
            // Entry road color
            if (entryRoad == 'A')
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
            else if (entryRoad == 'B')
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
            else if (entryRoad == 'C')
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
            else if (entryRoad == 'D')
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
        }
        else
        {
            // Exit road color
            if (exitRoad == 'A')
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
            else if (exitRoad == 'B')
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
            else if (exitRoad == 'C')
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
            else if (exitRoad == 'D')
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
        }

        SDL_FRect vehicleRect = {x, y, VEHICLE_SIZE, VEHICLE_SIZE};
        SDL_RenderFillRect(renderer, &vehicleRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(10); // ~60 FPS
    }
}