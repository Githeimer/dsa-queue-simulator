#include <stdio.h>
#include "generator.h"
#include "lane.h"
#include "vehicle.h"

// Include for sleep (platform-dependent)
#ifdef _WIN32
#include <windows.h> // For Sleep() on Windows
#else
#include <unistd.h> // For sleep() on Linux/macOS
#endif

int main()
{
    // Initialize a lane (Example: AL2)
    Lane al2;
    initLane(&al2, "AL2");

    // Initialize vehicle ID counter
    int vehicleIdCounter = 0;

    // Generate 10 vehicles for the lane AL2
    for (int i = 0; i < 10; i++)
    {
        generateVehicle(&al2, &vehicleIdCounter);
// Sleep for 1 second (platform dependent)
#ifdef _WIN32
        Sleep(1000); // 1000 ms = 1 second (Windows)
#else
        sleep(1); // 1 second (Linux/macOS)
#endif
    }

    // Write the generated vehicles to a file
    writeVehicleToFile(&al2, "AL2.txt");

    // Optionally, print the queue size or vehicle details
    printf("Generated vehicles in AL2:\n");
    Vehicle *current = al2.head;
    while (current != NULL)
    {
        printf("Vehicle ID: %d, Direction: %c\n", current->id, current->direction);
        current = current->next;
    }

    return 0;
}
