#ifndef VEHICLEQUEUE_H
#define VEHICLEQUEUE_H

// Define the Vehicle structure for simulation
typedef struct Vehicle
{
    int id;
    char entryLane[4];
    char exitLane[4];
    char direction[2];
    struct Vehicle *next;
} Vehicle;

// Define the Lane Queue structure
typedef struct LaneQueue
{
    Vehicle *front;
    Vehicle *rear;
    int size;
} LaneQueue;

// Function to initialize the queue
void initializeQueue(LaneQueue *queue);

// Function to enqueue a vehicle into the correct lane's queue
void enqueueVehicle(LaneQueue *queue, int id, const char *entryLane, const char *exitLane, const char *direction);

// Function to check the vehicle's entry lane and enqueue it accordingly
void checkQueue(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue,
                int id, const char *entryLane, const char *exitLane, const char *direction);

#endif // VEHICLEQUEUE_H
