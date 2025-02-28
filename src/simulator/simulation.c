#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vehicleQueue.h>
#include "road.h"
#include "animation.h"

#define WIDTH 900
#define HEIGHT 900
#define MAX_VEHICLES 100

typedef struct SimulationVehicle
{
    int id;
    char entryLane[4]; // AL2, BL2, etc.
    char exitLane[4];  // DL2, BL2, etc.
    char direction[2]; // Direction N, S, E, W
} SimulationVehicle;

Vehicle vehicles[MAX_VEHICLES];
int vehicleCount = 0;

LaneQueue AL2Queue;
LaneQueue BL2Queue;
LaneQueue CL2Queue;
LaneQueue DL2Queue;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int processedVehicleIDs[MAX_VEHICLES];
int processedVehicleCount = 0;

int isVehicleProcessed(int id)
{
    for (int i = 0; i < processedVehicleCount; i++)
    {
        if (processedVehicleIDs[i] == id)
        {
            return 1; // Vehicle already processed
        }
    }
    return 0; // New vehicle
}

void readVehicleData(const char *filename, SDL_Renderer *renderer)
{

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    char line[100];
    Vehicle newVehicles[MAX_VEHICLES];
    int newVehicleCount = 0;

    while (fgets(line, sizeof(line), file))
    {
        // Trim newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        int id;
        char entryLane[4]; // AL2, BL2, etc.
        char exitLane[4];  // DL2, BL2, etc.
        char direction[2]; // Direction N, S, E, W

        // Manually extract the vehicle details
        if (sscanf(line, "Vehicle ID: %d, Entry Lane: %3s, Exit Lane: %3s, Direction: %1s",
                   &id, entryLane, exitLane, direction) == 4)
        {
            if (!isVehicleProcessed(id))
            {
                // Add the vehicle to the new vehicles list
                newVehicles[newVehicleCount].id = id;
                strncpy(newVehicles[newVehicleCount].entryLane, entryLane, sizeof(newVehicles[newVehicleCount].entryLane) - 1);
                newVehicles[newVehicleCount].entryLane[sizeof(newVehicles[newVehicleCount].entryLane) - 1] = '\0';
                strncpy(newVehicles[newVehicleCount].exitLane, exitLane, sizeof(newVehicles[newVehicleCount].exitLane) - 1);
                newVehicles[newVehicleCount].exitLane[sizeof(newVehicles[newVehicleCount].exitLane) - 1] = '\0';
                strncpy(newVehicles[newVehicleCount].direction, direction, sizeof(newVehicles[newVehicleCount].direction) - 1);
                newVehicles[newVehicleCount].direction[sizeof(newVehicles[newVehicleCount].direction) - 1] = '\0';

                printf("Successfully parsed Vehicle - ID: %d, Entry Lane: %s, Exit Lane: %s, Direction: %s\n",
                       newVehicles[newVehicleCount].id, newVehicles[newVehicleCount].entryLane,
                       newVehicles[newVehicleCount].exitLane, newVehicles[newVehicleCount].direction);

                // Call checkQueue function to enqueue the vehicle in the correct lane
                checkQueue(&AL2Queue, &BL2Queue, &CL2Queue, &DL2Queue,
                           newVehicles[newVehicleCount].id, newVehicles[newVehicleCount].entryLane,
                           newVehicles[newVehicleCount].exitLane, newVehicles[newVehicleCount].direction, renderer);

                // Add the vehicle ID to the processed list
                processedVehicleIDs[processedVehicleCount++] = id;
                newVehicleCount++;
            }
        }
        else
        {
            printf("Failed to parse line: '%s'\n", line);
        }

        if (newVehicleCount >= MAX_VEHICLES)
            break;
    }

    fclose(file);

    // Update the main vehicle array with new vehicles
    for (int i = 0; i < newVehicleCount; i++)
    {
        vehicles[vehicleCount] = newVehicles[i];
        vehicleCount++;
        if (vehicleCount >= MAX_VEHICLES)
            break;
    }
}

void initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Traffic Simulator", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, NULL);

    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    // Initialize the queues for each lane
    initializeQueue(&AL2Queue);
    initializeQueue(&BL2Queue);
    initializeQueue(&CL2Queue);
    initializeQueue(&DL2Queue);

    // RenderVehicle(renderer, "AL2", "CL2");
}

void closeSDL()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    initSDL();

    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                closeSDL();
                return 0;
            }
        }
        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255); // Light gray color

        SDL_RenderClear(renderer);
        DrawRoad(renderer);

        // Read new vehicle data - this now just adds to queues without rendering
        readVehicleData("./data/vehicles.txt", renderer);

        // Update all vehicle positions
        renderAllVehicles(&AL2Queue, &BL2Queue, &CL2Queue, &DL2Queue, renderer);
        updateAllVehicles(&AL2Queue, &BL2Queue, &CL2Queue, &DL2Queue);

               SDL_RenderPresent(renderer);

        // Add a small delay to control frame rate
        SDL_Delay(16); // Approx 60 FPS
    }

    closeSDL();
    return 0;
}