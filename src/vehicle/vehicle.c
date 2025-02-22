#include "vehicle.h"
#include <stdio.h>
#include <stdlib.h>

// Create a new vehicle with the given attributes
Vehicle *createVehicle(int id, const char *entryLane, const char *exitLane, char direction)
{
    Vehicle *vehicle = (Vehicle *)malloc(sizeof(Vehicle));
    if (vehicle == NULL)
    {
        perror("Error allocating memory for vehicle");
        return NULL;
    }

    vehicle->id = id;
    snprintf(vehicle->entryLane, sizeof(vehicle->entryLane), "%s", entryLane);
    snprintf(vehicle->exitLane, sizeof(vehicle->exitLane), "%s", exitLane);
    vehicle->direction = direction;
    vehicle->state = 0; // Initial state is "queued"
    vehicle->next = NULL;

    return vehicle;
}

// Free the memory allocated for a vehicle
void destroyVehicle(Vehicle *vehicle)
{
    free(vehicle);
}
