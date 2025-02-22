#ifndef VEHICLE_H
#define VEHICLE_H

// Vehicle struct definition
typedef struct Vehicle
{
    int id;               // Unique vehicle ID
    char entryLane[4];    // Entry lane (e.g., "AL2", "BL2", etc.)
    char exitLane[4];     // Exit lane (e.g., "AL1", "L3", etc.)
    char direction;       // Direction ('N' for North, 'E' for East, 'S' for South, 'W' for West)
    int state;            // State (0 = queued, 1 = moving, 2 = passed)
    struct Vehicle *next; // For linked list
} Vehicle;

// Function prototypes for Vehicle management
Vehicle *createVehicle(int id, const char *entryLane, const char *exitLane, char direction);
void destroyVehicle(Vehicle *vehicle);

#endif // VEHICLE_H
