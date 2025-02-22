#include "lane.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize the lane with the given lane name
void initLane(Lane *lane, const char *laneName)
{
    snprintf(lane->name, sizeof(lane->name), "%s", laneName);
    lane->head = NULL;
    lane->tail = NULL;
}

// Enqueue a vehicle in the lane
void enqueue(Lane *lane, Vehicle *vehicle)
{
    if (lane->tail == NULL) // If the queue is empty
    {
        lane->head = vehicle;
        lane->tail = vehicle;
    }
    else // Otherwise, add to the end of the queue
    {
        lane->tail->next = vehicle;
        lane->tail = vehicle;
    }
}

// Dequeue a vehicle from the lane
Vehicle *dequeue(Lane *lane)
{
    if (lane->head == NULL)
        return NULL;

    Vehicle *vehicle = lane->head;
    lane->head = lane->head->next;
    if (lane->head == NULL) // If the queue is now empty
        lane->tail = NULL;

    vehicle->next = NULL; // Disconnect the vehicle from the queue
    return vehicle;
}
