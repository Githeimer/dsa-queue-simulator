#include "generator.h"
#include "lane.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Generates a random vehicle and enqueues it in the given lane
void generateVehicle(Lane *lane, int *vehicleIdCounter)
{
    // Define exit lanes (random choice between L1 and L3)
    const char *exitLanes[] = {"AL1", "BL1", "CL1", "DL1", "L3"};

    // Randomly choose an entry lane and exit lane
    char entryLane[4];
    snprintf(entryLane, sizeof(entryLane), "%s", lane->name); // Use the lane's name as the entry lane
    const char *exitLane = exitLanes[rand() % 5];             // Random exit lane

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
