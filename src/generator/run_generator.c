#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // For Sleep() on Windows

#include "generator.h"
#include "vehicle.h"
#include "lane.h"

// Define the list of lanes
const char *lanes[] = {"AL2", "BL2", "CL2", "DL2"};

int main()
{
    // Initialize random seed for randomness
    srand(time(NULL));

    // Initialize vehicle ID counter (ensure unique vehicle IDs)
    int vehicleIdCounter = 0;

    // Initialize an array of Lane structs to store the lanes
    Lane laneObjects[4];
    for (int i = 0; i < 4; i++)
    {
        initLane(&laneObjects[i], lanes[i]);
    }

    // Infinite loop to continuously generate vehicles
    while (1)
    {
        // Choose a random lane
        int laneIndex = rand() % 4;
        printf("\nSelected lane: %s\n", lanes[laneIndex]);

        // Generate a new vehicle and get the unique vehicle ID
        generateVehicle(&laneObjects[laneIndex], &vehicleIdCounter);

        // Store the vehicle immediately in the file
        char fileName[20]; // Change to accommodate longer file names
        snprintf(fileName, sizeof(fileName), "./data/vehicles.txt");

        // Write to file
        writeNewVehicleToFile(&laneObjects[laneIndex], "./data/vehicles.txt");

        // Print a message for the generated vehicle
        printf("Vehicle %d generated in lane %s\n", vehicleIdCounter - 1, lanes[laneIndex]);

        Sleep(3500); // Sleep  (7.5 seconds)
    }

    return 0;
}
