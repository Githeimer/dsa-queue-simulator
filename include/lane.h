#ifndef LANE_H
#define LANE_H

#include "vehicle.h"

// Lane struct to hold vehicle queue
typedef struct Lane
{
    char name[10]; // Lane name (e.g., "AL2", "BL2", etc.)
    Vehicle *head; // Head of the queue
    Vehicle *tail; // Tail of the queue
} Lane;

// Function prototypes
void initLane(Lane *lane, const char *laneName);
void enqueue(Lane *lane, Vehicle *vehicle);
Vehicle *dequeue(Lane *lane);

#endif // LANE_H
