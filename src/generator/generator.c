#include "generator.h"
#include "lane.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Generates a random vehicle and enqueues it in the given lane
void generateVehicle(Lane *lane, int *vehicleIdCounter)
{

    const char *exitLanesAL2[] = {"CL1", "DL1"};
    const char *exitLanesBL2[] = {"DL1", "AL1"};
    const char *exitLanesCL2[] = {"AL1", "BL1"};
    const char *exitLanesDL2[] = {"BL1", "CL1"};

    // Randomly choose an entry lane
    char entryLane[4];
    snprintf(entryLane, sizeof(entryLane), "%s", lane->name); // Use the lane's name as the entry lane

    // Define the valid exit lanes based on the entry lane
    const char **exitLanes = NULL;
    int numExitLanes = 0;

    if (strcmp(entryLane, "AL2") == 0)
    {
        exitLanes = exitLanesAL2;
        numExitLanes = 2;
    }
    else if (strcmp(entryLane, "BL2") == 0)
    {
        exitLanes = exitLanesBL2;
        numExitLanes = 2;
    }
    else if (strcmp(entryLane, "CL2") == 0)
    {
        exitLanes = exitLanesCL2;
        numExitLanes = 2;
    }
    else if (strcmp(entryLane, "DL2") == 0)
    {
        exitLanes = exitLanesDL2;
        numExitLanes = 2;
    }

    // Randomly choose an exit lane
    const char *exitLane = exitLanes[rand() % numExitLanes];

    // Randomly pick a direction (N, E, S, W)
    const char directions[] = {'N', 'E', 'S', 'W'};
    char direction = directions[rand() % 4];

    // Create the vehicle with a unique ID
    Vehicle *vehicle = createVehicle(*vehicleIdCounter, entryLane, exitLane, direction);

    // Enqueue the vehicle in the lane
    enqueue(lane, vehicle);

    // Increment vehicle ID counter
    (*vehicleIdCounter)++;
}

// Write the details of the most recently generated vehicle to the file
void writeNewVehicleToFile(Lane *lane, const char *fileName)
{
    FILE *file = fopen(fileName, "a"); // Open in append mode

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Get the most recently generated vehicle (tail of the queue)
    Vehicle *newVehicle = lane->tail;
    if (newVehicle != NULL)
    {
        fprintf(file, "Vehicle ID: %d, Entry Lane: %s, Exit Lane: %s, Direction: %c\n",
                newVehicle->id, newVehicle->entryLane, newVehicle->exitLane, newVehicle->direction);
    }

    fclose(file); // Close the file after writing
}
