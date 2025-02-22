#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_VEHICLES 100

typedef struct Vehicle
{
    int id;
    char entryLane[4]; // AL2, BL2, etc.
    char exitLane[4];  // DL2, BL2, etc.
    char direction[2]; // Direction N, S, E, W
} Vehicle;

Vehicle vehicles[MAX_VEHICLES];
int vehicleCount = 0;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// A simple array to keep track of processed vehicle IDs.
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

void readVehicleData(const char *filename)
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
    window = SDL_CreateWindow("Traffic Simulator", WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, "renderer");
}

void renderVehicles()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for vehicles

    for (int i = 0; i < vehicleCount; i++)
    {
        // Example vehicle positioning based on direction (this can be made more advanced)
        float x = 100 + i * 40;
        float y = 100;

        if (strcmp(vehicles[i].direction, "N") == 0)
        {
            y -= 20; // Moving North
        }
        else if (strcmp(vehicles[i].direction, "S") == 0)
        {
            y += 20; // Moving South
        }
        else if (strcmp(vehicles[i].direction, "E") == 0)
        {
            x += 20; // Moving East
        }
        else if (strcmp(vehicles[i].direction, "W") == 0)
        {
            x -= 20; // Moving West
        }

        SDL_FRect vehicleRect = {x, y, 20, 10};
        SDL_RenderFillRect(renderer, &vehicleRect);
    }
}

void renderTrafficLights()
{
    // Placeholder for traffic lights
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green light (For example)
    SDL_FRect lightRect = {WIDTH / 2 - 25, HEIGHT / 2 - 25, 50, 50};
    SDL_RenderFillRect(renderer, &lightRect);
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

        SDL_RenderClear(renderer);

        // Read vehicle data from the file
        readVehicleData("./data/vehicles.txt");

        renderVehicles();
        renderTrafficLights();

        SDL_RenderPresent(renderer);

        SDL_Delay(4100); // Delay to simulate real-time (2.1 seconds)
    }

    closeSDL();
    return 0;
}
