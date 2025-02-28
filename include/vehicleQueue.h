#ifndef VEHICLEQUEUE_H
#define VEHICLEQUEUE_H

#include <SDL3/SDL.h>

typedef enum
{
    RED,
    GREEN,
    YELLOW
} TrafficLightState;
// Define the Vehicle structure for simulation
typedef struct Vehicle
{
    int id;
    char entryLane[4];
    char exitLane[4];
    char direction[2];

    // New fields for animation
    int x, y;       // Current position
    float progress; // Animation progress (0.0 to 1.0)
    bool active;    // Whether this vehicle is currently being animated

    // Path information
    int startX, startY; // Starting coordinates
    int endX, endY;     // Ending coordinates

    struct Vehicle *next;
} Vehicle;
// Define the Lane Queue structure

typedef struct
{
    TrafficLightState state;
    int greenTimer;   // Counts down remaining green time
    int yellowTimer;  // Counts down remaining yellow time
    int minGreenTime; // Minimum green time
    int maxGreenTime; // Maximum green time
    int yellowTime;   // Standard yellow time
} TrafficLight;

typedef struct
{
    Vehicle *front;
    Vehicle *rear;
    int size;
    TrafficLight light; // Add traffic light to each lane
} LaneQueue;

// Function to initialize the queue
void initializeQueue(LaneQueue *queue);

// Function to enqueue a vehicle into the correct lane's queue
void enqueueVehicle(LaneQueue *queue, int id, const char *entryLane, const char *exitLane, const char *direction, SDL_Renderer *renderer);

void dequeueVehicle(LaneQueue *queue);

// Function to check the vehicle's entry lane and enqueue it accordingly
void checkQueue(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue,
                int id, const char *entryLane, const char *exitLane, const char *direction, SDL_Renderer *renderer);

void updateAllVehicles(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue);

void initializeVehiclePosition(Vehicle *vehicle, const char *entryLane, const char *exitLane);

void updateVehiclesInQueue(LaneQueue *queue);

void renderAllVehicles(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue, SDL_Renderer *renderer);

void renderVehiclesInQueue(LaneQueue *queue, SDL_Renderer *renderer);

void drawVehicle(SDL_Renderer *renderer, int x, int y, const char *direction);

void updateVehiclePositionBasedOnPath(Vehicle *vehicle, int centerX, int centerY);
void initializeTrafficLight(TrafficLight *light);
void updateTrafficLights(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue);
void switchLightToGreen(LaneQueue *laneQueue, LaneQueue *otherLanes[], int numOtherLanes);
void renderTrafficLights(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue, SDL_Renderer *renderer);

#endif // VEHICLEQUEUE_H
