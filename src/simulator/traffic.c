#include "vehicleQueue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "animation.h"
#include <time.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900
#define ROAD_WIDTH 180  // Each road's total width (3 lanes x 60 width)
#define LANE_WIDTH 60   // Width of each lane
#define VEHICLE_SIZE 30 // Vehicle size (width and height)

void initializeTrafficLight(TrafficLight *light)
{
    light->state = RED;
    light->greenTimer = 0;
    light->yellowTimer = 0;
    light->minGreenTime = 5 * 60;  // 5 seconds at 60 FPS
    light->maxGreenTime = 15 * 60; // 15 seconds at 60 FPS
    light->yellowTime = 3 * 60;    // 3 seconds at 60 FPS
}

void updateTrafficLights(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue)
{
    static int tickCounter = 0;
    static bool isFirstRun = true;
    LaneQueue *currentGreenLane = NULL;

    // On first run, randomly select a lane to start with green light
    if (isFirstRun)
    {
        srand(time(NULL));
        int randomLane = rand() % 4;

        LaneQueue *lanes[4] = {AL2Queue, BL2Queue, CL2Queue, DL2Queue};
        LaneQueue *otherLanes[3];
        int otherLaneIndex = 0;

        for (int i = 0; i < 4; i++)
        {
            if (i != randomLane)
            {
                otherLanes[otherLaneIndex++] = lanes[i];
            }
        }

        switchLightToGreen(lanes[randomLane], otherLanes, 3);
        isFirstRun = false;
        return;
    }

    // Check if AL2 has more than 10 vehicles and prioritize it
    if (AL2Queue->size > 10 && AL2Queue->light.state == RED)
    {
        LaneQueue *otherLanes[3] = {BL2Queue, CL2Queue, DL2Queue};

        // Only switch to AL2 if no yellow lights are active
        bool canSwitch = true;
        for (int i = 0; i < 3; i++)
        {
            if (otherLanes[i]->light.state == YELLOW)
            {
                canSwitch = false;
                break;
            }
        }

        if (canSwitch)
        {
            printf("AL2 queue has %d vehicles - prioritizing!\n", AL2Queue->size);
            switchLightToGreen(AL2Queue, otherLanes, 3);
            return;
        }
    }

    // Handle ongoing light states
    if (AL2Queue->light.state == GREEN)
        currentGreenLane = AL2Queue;
    else if (BL2Queue->light.state == GREEN)
        currentGreenLane = BL2Queue;
    else if (CL2Queue->light.state == GREEN)
        currentGreenLane = CL2Queue;
    else if (DL2Queue->light.state == GREEN)
        currentGreenLane = DL2Queue;

    // If a lane has a green light
    if (currentGreenLane != NULL)
    {
        // Decrement the green timer
        currentGreenLane->light.greenTimer--;

        // If green time is up, switch to yellow
        if (currentGreenLane->light.greenTimer <= 0)
        {
            currentGreenLane->light.state = YELLOW;
            currentGreenLane->light.yellowTimer = currentGreenLane->light.yellowTime;
            printf("Switching %p to YELLOW\n", (void *)currentGreenLane);
        }
    }

    // Check for yellow lights
    LaneQueue *lanes[4] = {AL2Queue, BL2Queue, CL2Queue, DL2Queue};
    LaneQueue *yellowLane = NULL;

    for (int i = 0; i < 4; i++)
    {
        if (lanes[i]->light.state == YELLOW)
        {
            yellowLane = lanes[i];
            break;
        }
    }

    // If a lane has a yellow light
    if (yellowLane != NULL)
    {
        // Decrement the yellow timer
        yellowLane->light.yellowTimer--;

        // If yellow time is up, switch to the next lane
        if (yellowLane->light.yellowTimer <= 0)
        {
            // Find which lanes aren't the yellow lane
            LaneQueue *otherLanes[3];
            int otherLaneIndex = 0;
            LaneQueue *nextGreenLane = NULL;

            // Default next lane selection (can be random or sequence)
            int nextLaneIndex = -1;

            // If AL2 has > 10 vehicles, prioritize it
            if (AL2Queue->size > 10 && yellowLane != AL2Queue)
            {
                nextGreenLane = AL2Queue;
            }
            else
            {
                // Simple cycling through lanes
                for (int i = 0; i < 4; i++)
                {
                    if (lanes[i] == yellowLane)
                    {
                        nextLaneIndex = (i + 1) % 4;
                        break;
                    }
                }
                nextGreenLane = lanes[nextLaneIndex];
            }

            // Get the other lanes
            for (int i = 0; i < 4; i++)
            {
                if (lanes[i] != nextGreenLane)
                {
                    otherLanes[otherLaneIndex++] = lanes[i];
                }
            }

            // Switch the next lane to green
            switchLightToGreen(nextGreenLane, otherLanes, 3);
        }
    }

    // Increment tick counter and occasionally print debug info
    tickCounter++;
    if (tickCounter % 60 == 0)
    { // Once per second at 60 FPS
        printf("Traffic status: AL2=%d (%s), BL2=%d (%s), CL2=%d (%s), DL2=%d (%s)\n",
               AL2Queue->size,
               AL2Queue->light.state == RED ? "RED" : (AL2Queue->light.state == GREEN ? "GREEN" : "YELLOW"),
               BL2Queue->size,
               BL2Queue->light.state == RED ? "RED" : (BL2Queue->light.state == GREEN ? "GREEN" : "YELLOW"),
               CL2Queue->size,
               CL2Queue->light.state == RED ? "RED" : (CL2Queue->light.state == GREEN ? "GREEN" : "YELLOW"),
               DL2Queue->size,
               DL2Queue->light.state == RED ? "RED" : (DL2Queue->light.state == GREEN ? "GREEN" : "YELLOW"));
    }
}

void switchLightToGreen(LaneQueue *laneQueue, LaneQueue *otherLanes[], int numOtherLanes)
{
    // Set other lanes to red
    for (int i = 0; i < numOtherLanes; i++)
    {
        otherLanes[i]->light.state = RED;
    }

    // Set the selected lane to green with a random time between min and max
    laneQueue->light.state = GREEN;

    // Base green time on queue size - longer time for longer queues
    int queueFactor = laneQueue->size * 60; // 1 second per car (at 60 FPS)
    int baseGreenTime = laneQueue->light.minGreenTime;

    // Calculate green time based on queue size, capped at max green time
    laneQueue->light.greenTimer = baseGreenTime + queueFactor;
    if (laneQueue->light.greenTimer > laneQueue->light.maxGreenTime)
    {
        laneQueue->light.greenTimer = laneQueue->light.maxGreenTime;
    }

    printf("Switching lane %p to GREEN for %d ticks\n", (void *)laneQueue, laneQueue->light.greenTimer);
}

void renderTrafficLights(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue, SDL_Renderer *renderer)
{
    // Calculate intersection center
    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;
    int roadWidth = ROAD_WIDTH;

    // Traffic light size and positioning
    int lightSize = 20;
    int offset = 30;

    // Draw traffic lights at the four entry points

    // North traffic light (A road)
    SDL_FRect northLight = {centerX - lightSize / 2, centerY - roadWidth / 2 - offset, lightSize, lightSize};
    if (AL2Queue->light.state == RED)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    else if (AL2Queue->light.state == YELLOW)
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &northLight);

    // East traffic light (B road)
    SDL_FRect eastLight = {centerX + roadWidth / 2 + offset, centerY - lightSize / 2, lightSize, lightSize};
    if (BL2Queue->light.state == RED)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    else if (BL2Queue->light.state == YELLOW)
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &eastLight);

    // South traffic light (C road)
    SDL_FRect southLight = {centerX - lightSize / 2, centerY + roadWidth / 2 + offset, lightSize, lightSize};
    if (CL2Queue->light.state == RED)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    else if (CL2Queue->light.state == YELLOW)
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &southLight);

    // West traffic light (D road)
    SDL_FRect westLight = {centerX - roadWidth / 2 - offset, centerY - lightSize / 2, lightSize, lightSize};
    if (DL2Queue->light.state == RED)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    else if (DL2Queue->light.state == YELLOW)
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &westLight);
}
