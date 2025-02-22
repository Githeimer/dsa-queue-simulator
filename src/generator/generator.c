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

    // Create the vehicle
    Vehicle *vehicle = createVehicle(*vehicleIdCounter, entryLane, exitLane, direction);

    // Enqueue the vehicle in the lane
    enqueue(lane, vehicle);

    // Increment vehicle ID counter
    (*vehicleIdCounter)++;
}

// Write the vehicle details to the file associated with the lane
void writeVehicleToFile(Lane *lane, const char *fileName)
{
    FILE *file = fopen(fileName, "a"); // Open in append mode

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    Vehicle *current = lane->head;
    while (current != NULL)
    {
        fprintf(file, "Vehicle ID: %d, Entry Lane: %s, Exit Lane: %s, Direction: %c\n",
                current->id, current->entryLane, current->exitLane, current->direction);
        current = current->next;
    }

    fclose(file); // Close the file after writing
}
