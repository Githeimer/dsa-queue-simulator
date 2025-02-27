#include "vehicleQueue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Initialize the queue for each lane
void initializeQueue(LaneQueue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

// Create a new vehicle node
Vehicle *createVehicleNode(int id, const char *entryLane, const char *exitLane, const char *direction)
{
    Vehicle *newVehicle = (Vehicle *)malloc(sizeof(Vehicle));
    if (!newVehicle)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newVehicle->id = id;
    strncpy(newVehicle->entryLane, entryLane, sizeof(newVehicle->entryLane) - 1);
    newVehicle->entryLane[sizeof(newVehicle->entryLane) - 1] = '\0';
    strncpy(newVehicle->exitLane, exitLane, sizeof(newVehicle->exitLane) - 1);
    newVehicle->exitLane[sizeof(newVehicle->exitLane) - 1] = '\0';
    strncpy(newVehicle->direction, direction, sizeof(newVehicle->direction) - 1);
    newVehicle->direction[sizeof(newVehicle->direction) - 1] = '\0';
    newVehicle->next = NULL;

    return newVehicle;
}

// Enqueue a vehicle into the lane's queue
void enqueueVehicle(LaneQueue *queue, int id, const char *entryLane, const char *exitLane, const char *direction)
{
    Vehicle *newVehicle = createVehicleNode(id, entryLane, exitLane, direction);
    if (!newVehicle)
        return;

    if (queue->rear == NULL)
    {
        queue->front = queue->rear = newVehicle;
        return;
    }

    queue->rear->next = newVehicle;
    queue->rear = newVehicle;
    queue->size = queue->size + 1;

    // renderVehicle(newVehicle);
}

// Check the entry lane and enqueue the vehicle
void checkQueue(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue,
                int id, const char *entryLane, const char *exitLane, const char *direction)
{
    if (strcmp(entryLane, "AL2") == 0)
    {
        enqueueVehicle(AL2Queue, id, entryLane, exitLane, direction);
        printf("Enqueued vehicle with ID %d to AL2\n", id);
    }
    else if (strcmp(entryLane, "BL2") == 0)
    {
        enqueueVehicle(BL2Queue, id, entryLane, exitLane, direction);
        printf("Enqueued vehicle with ID %d to BL2\n", id);
    }
    else if (strcmp(entryLane, "CL2") == 0)
    {
        enqueueVehicle(DL2Queue, id, entryLane, exitLane, direction);
        printf("Enqueued vehicle with ID %d to DL2\n", id);
    }
    else if (strcmp(entryLane, "DL2") == 0)
    {
        enqueueVehicle(DL2Queue, id, entryLane, exitLane, direction);
        printf("Enqueued vehicle with ID %d to DL2\n", id);
    }
    else
    {
        printf("No matching entry lane for vehicle ID %d\n", id);
    }
}
