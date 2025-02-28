#include "vehicleQueue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "animation.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900
#define ROAD_WIDTH 180  // Each road's total width (3 lanes x 60 width)
#define LANE_WIDTH 60   // Width of each lane
#define VEHICLE_SIZE 30 // Vehicle size (width and height)

// Initialize the queue for each lane
void initializeQueue(LaneQueue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    initializeTrafficLight(&queue->light);
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

void enqueueVehicle(LaneQueue *queue, int id, const char *entryLane, const char *exitLane, const char *direction, SDL_Renderer *renderer)
{
    Vehicle *newVehicle = createVehicleNode(id, entryLane, exitLane, direction);
    if (!newVehicle)
        return;

    initializeVehiclePosition(newVehicle, entryLane, exitLane);

    newVehicle->active = true;
    newVehicle->progress = 0.0f;

    if (queue->rear == NULL)
    {
        queue->front = queue->rear = newVehicle;
    }
    else
    {
        queue->rear->next = newVehicle;
        queue->rear = newVehicle;
    }
    queue->size = queue->size + 1;
}

void checkQueue(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue, int id, const char *entryLane, const char *exitLane, const char *direction, SDL_Renderer *renderer)
{
    if (strcmp(entryLane, "AL2") == 0)
    {
        enqueueVehicle(AL2Queue, id, entryLane, exitLane, direction, renderer);
        printf("Enqueued vehicle with ID %d to AL2\n", id);
    }
    else if (strcmp(entryLane, "BL2") == 0)
    {
        enqueueVehicle(BL2Queue, id, entryLane, exitLane, direction, renderer);
        printf("Enqueued vehicle with ID %d to BL2\n", id);
    }
    else if (strcmp(entryLane, "CL2") == 0)
    {
        enqueueVehicle(CL2Queue, id, entryLane, exitLane, direction, renderer);
        printf("Enqueued vehicle with ID %d to DL2\n", id);
    }
    else if (strcmp(entryLane, "DL2") == 0)
    {
        enqueueVehicle(DL2Queue, id, entryLane, exitLane, direction, renderer);
        printf("Enqueued vehicle with ID %d to DL2\n", id);
    }
    else
    {
        printf("No matching entry lane for vehicle ID %d\n", id);
    }
}

void updateAllVehicles(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue)
{
    updateTrafficLights(AL2Queue, BL2Queue, CL2Queue, DL2Queue);
    // Update vehicles in each queue
    updateVehiclesInQueue(AL2Queue);
    updateVehiclesInQueue(BL2Queue);
    updateVehiclesInQueue(CL2Queue);
    updateVehiclesInQueue(DL2Queue);
}

void renderAllVehicles(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue, SDL_Renderer *renderer)
{

    renderVehiclesInQueue(AL2Queue, renderer);
    renderVehiclesInQueue(BL2Queue, renderer);
    renderVehiclesInQueue(CL2Queue, renderer);
    renderVehiclesInQueue(DL2Queue, renderer);
    renderTrafficLights(AL2Queue, BL2Queue, CL2Queue, DL2Queue, renderer);
}

void renderVehiclesInQueue(LaneQueue *queue, SDL_Renderer *renderer)
{
    Vehicle *current = queue->front;

    while (current != NULL)
    {
        if (current->active)
        {
            // Draw vehicle at its current position
            drawVehicle(renderer, current->x, current->y, current->direction);
        }
        current = current->next;
    }
}

void dequeueVehicle(LaneQueue *queue)
{
    if (queue->front == NULL)
        return;

    Vehicle *temp = queue->front;
    queue->front = queue->front->next;

    // If queue becomes empty
    if (queue->front == NULL)
        queue->rear = NULL;

    queue->size--;
    free(temp);
}

void initializeVehiclePosition(Vehicle *vehicle, const char *entryLane, const char *exitLane)
{
    // Calculate intersection center
    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;

    // Calculate road positions
    int roadLeftEdge = centerX - ROAD_WIDTH / 2;
    int roadRightEdge = centerX + ROAD_WIDTH / 2;
    int roadTopEdge = centerY - ROAD_WIDTH / 2;
    int roadBottomEdge = centerY + ROAD_WIDTH / 2;

    // Entry points - more consistent positioning
    if (strcmp(entryLane, "AL1") == 0)
    {
        vehicle->startX = roadLeftEdge + LANE_WIDTH / 2; // Leftmost lane of A
        vehicle->startY = 0;
    }
    else if (strcmp(entryLane, "AL2") == 0)
    {
        vehicle->startX = roadLeftEdge + LANE_WIDTH + LANE_WIDTH / 2; // Middle lane of A
        vehicle->startY = 0;
    }
    else if (strcmp(entryLane, "AL3") == 0)
    {
        vehicle->startX = roadLeftEdge + 2 * LANE_WIDTH + LANE_WIDTH / 2; // Rightmost lane of A
        vehicle->startY = 0;
    }
    else if (strcmp(entryLane, "BL1") == 0)
    {
        vehicle->startX = WINDOW_WIDTH;
        vehicle->startY = roadTopEdge + LANE_WIDTH / 2; // Top lane of B
    }
    else if (strcmp(entryLane, "BL2") == 0)
    {
        vehicle->startX = WINDOW_WIDTH;
        vehicle->startY = roadTopEdge + LANE_WIDTH + LANE_WIDTH / 2; // Middle lane of B
    }
    else if (strcmp(entryLane, "BL3") == 0)
    {
        vehicle->startX = WINDOW_WIDTH;
        vehicle->startY = roadTopEdge + 2 * LANE_WIDTH + LANE_WIDTH / 2; // Bottom lane of B
    }
    else if (strcmp(entryLane, "CL1") == 0)
    {
        vehicle->startX = roadLeftEdge + LANE_WIDTH / 2; // Leftmost lane of C
        vehicle->startY = WINDOW_HEIGHT;
    }
    else if (strcmp(entryLane, "CL2") == 0)
    {
        vehicle->startX = roadLeftEdge + LANE_WIDTH + LANE_WIDTH / 2; // Middle lane of C
        vehicle->startY = WINDOW_HEIGHT;
    }
    else if (strcmp(entryLane, "CL3") == 0)
    {
        vehicle->startX = roadLeftEdge + 2 * LANE_WIDTH + LANE_WIDTH / 2; // Rightmost lane of C
        vehicle->startY = WINDOW_HEIGHT;
    }
    else if (strcmp(entryLane, "DL1") == 0)
    {
        vehicle->startX = 0;
        vehicle->startY = roadTopEdge + 2 * LANE_WIDTH + LANE_WIDTH / 2; // Bottom lane of D (corrected)
    }
    else if (strcmp(entryLane, "DL2") == 0)
    {
        vehicle->startX = 0;
        vehicle->startY = roadTopEdge + LANE_WIDTH + LANE_WIDTH / 2; // Middle lane of D
    }
    else if (strcmp(entryLane, "DL3") == 0)
    {
        vehicle->startX = 0;
        vehicle->startY = roadTopEdge + LANE_WIDTH / 2; // Top lane of D (corrected)
    }

    // Fix the exit lane positions - similar approach for consistency
    if (strcmp(exitLane, "AL1") == 0)
    {
        vehicle->endX = roadLeftEdge + LANE_WIDTH / 2;
        vehicle->endY = 0;
    }
    else if (strcmp(exitLane, "AL2") == 0)
    {
        vehicle->endX = roadLeftEdge + LANE_WIDTH + LANE_WIDTH / 2;
        vehicle->endY = 0;
    }
    else if (strcmp(exitLane, "AL3") == 0)
    {
        vehicle->endX = roadLeftEdge + 2 * LANE_WIDTH + LANE_WIDTH / 2;
        vehicle->endY = 0;
    }
    else if (strcmp(exitLane, "BL1") == 0)
    {
        vehicle->endX = WINDOW_WIDTH;
        vehicle->endY = roadTopEdge + LANE_WIDTH / 2;
    }
    else if (strcmp(exitLane, "BL2") == 0)
    {
        vehicle->endX = WINDOW_WIDTH;
        vehicle->endY = roadTopEdge + LANE_WIDTH + LANE_WIDTH / 2;
    }
    else if (strcmp(exitLane, "BL3") == 0)
    {
        vehicle->endX = WINDOW_WIDTH;
        vehicle->endY = roadTopEdge + 2 * LANE_WIDTH + LANE_WIDTH / 2;
    }
    else if (strcmp(exitLane, "CL1") == 0)
    {
        vehicle->endX = roadLeftEdge + LANE_WIDTH / 2;
        vehicle->endY = WINDOW_HEIGHT;
    }
    else if (strcmp(exitLane, "CL2") == 0)
    {
        vehicle->endX = roadLeftEdge + LANE_WIDTH + LANE_WIDTH / 2;
        vehicle->endY = WINDOW_HEIGHT;
    }
    else if (strcmp(exitLane, "CL3") == 0)
    {
        vehicle->endX = roadLeftEdge + 2 * LANE_WIDTH + LANE_WIDTH / 2;
        vehicle->endY = WINDOW_HEIGHT;
    }
    else if (strcmp(exitLane, "DL1") == 0)
    {
        vehicle->endX = 0;
        vehicle->endY = roadTopEdge + 2 * LANE_WIDTH + LANE_WIDTH / 2; // Bottom lane of D (corrected)
    }
    else if (strcmp(exitLane, "DL2") == 0)
    {
        vehicle->endX = 0;
        vehicle->endY = roadTopEdge + LANE_WIDTH + LANE_WIDTH / 2;
    }
    else if (strcmp(exitLane, "DL3") == 0)
    {
        vehicle->endX = 0;
        vehicle->endY = roadTopEdge + LANE_WIDTH / 2; // Top lane of D (corrected)
    }

    // Initialize current position to starting position
    vehicle->x = vehicle->startX;
    vehicle->y = vehicle->startY;
}

void drawVehicle(SDL_Renderer *renderer, int x, int y, const char *direction)
{
    // Set color based on direction
    if (strcmp(direction, "N") == 0)
    {
        SDL_SetRenderDrawColor(renderer, 143, 0, 255, 255);
    }
    else if (strcmp(direction, "E") == 0)
    {
        SDL_SetRenderDrawColor(renderer, 33, 79, 198, 255);
    }
    else if (strcmp(direction, "S") == 0)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue for South
    }
    else if (strcmp(direction, "W") == 0)
    {
        SDL_SetRenderDrawColor(renderer, 255, 127, 0, 255); // Cyan
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 8, 133, 161, 255);
    }

    // Create vehicle shape (rectangle + direction indicator)
    SDL_FRect vehicleRect = {x - VEHICLE_SIZE / 2, y - VEHICLE_SIZE / 2, VEHICLE_SIZE, VEHICLE_SIZE};
    SDL_RenderFillRect(renderer, &vehicleRect);

    // Add direction indicator (triangle)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for the direction indicator

    if (strcmp(direction, "N") == 0)
    {
        // North-pointing triangle
        SDL_RenderLine(renderer, x, y - VEHICLE_SIZE / 2, x - VEHICLE_SIZE / 4, y);
        SDL_RenderLine(renderer, x, y - VEHICLE_SIZE / 2, x + VEHICLE_SIZE / 4, y);
        SDL_RenderLine(renderer, x - VEHICLE_SIZE / 4, y, x + VEHICLE_SIZE / 4, y);
    }
    else if (strcmp(direction, "E") == 0)
    {
        // East-pointing triangle
        SDL_RenderLine(renderer, x + VEHICLE_SIZE / 2, y, x, y - VEHICLE_SIZE / 4);
        SDL_RenderLine(renderer, x + VEHICLE_SIZE / 2, y, x, y + VEHICLE_SIZE / 4);
        SDL_RenderLine(renderer, x, y - VEHICLE_SIZE / 4, x, y + VEHICLE_SIZE / 4);
    }
    else if (strcmp(direction, "S") == 0)
    {
        // South-pointing triangle
        SDL_RenderLine(renderer, x, y + VEHICLE_SIZE / 2, x - VEHICLE_SIZE / 4, y);
        SDL_RenderLine(renderer, x, y + VEHICLE_SIZE / 2, x + VEHICLE_SIZE / 4, y);
        SDL_RenderLine(renderer, x - VEHICLE_SIZE / 4, y, x + VEHICLE_SIZE / 4, y);
    }
    else if (strcmp(direction, "W") == 0)
    {
        // West-pointing triangle
        SDL_RenderLine(renderer, x - VEHICLE_SIZE / 2, y, x, y - VEHICLE_SIZE / 4);
        SDL_RenderLine(renderer, x - VEHICLE_SIZE / 2, y, x, y + VEHICLE_SIZE / 4);
        SDL_RenderLine(renderer, x, y - VEHICLE_SIZE / 4, x, y + VEHICLE_SIZE / 4);
    }
}

void updateVehiclesInQueue(LaneQueue *queue)
{
    if (!queue)
        return; // Null check

    Vehicle *current = queue->front;
    Vehicle *prev = NULL;

    // Traffic light state
    bool canMoveThrough = (queue->light.state == GREEN || queue->light.state == YELLOW);

    // Constants for vehicle movement
    const float VEHICLE_SPEED = 0.002f;
    const float MIN_VEHICLE_GAP = 0.05f;
    const float VEHICLE_LENGTH = 0.03f;

    // Calculate intersection center (once for efficiency)
    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;

    while (current != NULL)
    {
        if (!current->active)
        {
            prev = current;
            current = current->next;
            continue;
        }

        // Check if vehicle is at different points of its journey
        bool atIntersectionEntrance = (current->progress >= 0.39f && current->progress < 0.41f);
        bool inIntersection = (current->progress >= 0.4f && current->progress <= 0.6f);

        // Check spacing with vehicle ahead (if any)
        bool shouldMaintainGap = false;
        if (prev != NULL && prev->active)
        {
            // Verify the vehicle is ahead, not behind
            if (prev->progress > current->progress)
            {
                float gap = prev->progress - current->progress;
                if (gap < (MIN_VEHICLE_GAP + VEHICLE_LENGTH))
                {
                    shouldMaintainGap = true;
                }
            }
        }

        // Determine if vehicle should move
        bool shouldMove = true;

        // Stop at red light if at intersection entrance
        if (atIntersectionEntrance && !canMoveThrough)
        {
            shouldMove = false;
        }

        // Maintain gap with vehicle ahead
        if (shouldMaintainGap)
        {
            shouldMove = false;
        }

        // Move vehicle if conditions allow
        if (shouldMove)
        {
            current->progress += VEHICLE_SPEED;

            // Update vehicle position based on its progress and path
            if (current->progress >= 1.0f)
            {
                // Vehicle has completed its journey
                current->active = false;

                // Only dequeue if this is the front vehicle
                if (current == queue->front)
                {
                    Vehicle *temp = current;
                    current = current->next;
                    queue->front = current;
                    if (queue->front == NULL)
                        queue->rear = NULL;
                    queue->size--;
                    free(temp);
                    prev = NULL; // Reset prev since we've removed the front
                    continue;
                }
            }
            else
            {
                // Update position based on progress
                updateVehiclePositionBasedOnPath(current, centerX, centerY);
            }
        }

        // Move to next vehicle
        prev = current;
        current = current->next;
    }
}

// Helper function to update vehicle position based on its progress and path
void updateVehiclePositionBasedOnPath(Vehicle *vehicle, int centerX, int centerY)
{
    if (!vehicle)
        return;

    // Get entry and exit information
    char entryRoad = vehicle->entryLane[0];
    char exitRoad = vehicle->exitLane[0];
    int entryLaneNum = vehicle->entryLane[2] - '0';
    int exitLaneNum = vehicle->exitLane[2] - '0';

    // Check which part of the journey the vehicle is in
    if (vehicle->progress < 0.4f)
    {
        // Approaching intersection (first 40% of journey)
        float adjustedProgress = vehicle->progress / 0.4f;
        vehicle->x = vehicle->startX + (centerX - vehicle->startX) * adjustedProgress;
        vehicle->y = vehicle->startY + (centerY - vehicle->startY) * adjustedProgress;
    }
    else if (vehicle->progress <= 0.6f)
    {
        // Going through intersection (20% of journey)
        float turnProgress = (vehicle->progress - 0.4f) / 0.2f; // 0.0 to 1.0

        // Determine movement type based on entry/exit roads
        bool isStraight = ((entryRoad == 'A' && exitRoad == 'C') ||
                           (entryRoad == 'C' && exitRoad == 'A') ||
                           (entryRoad == 'B' && exitRoad == 'D') ||
                           (entryRoad == 'D' && exitRoad == 'B'));

        bool isRightTurn = ((entryRoad == 'A' && exitRoad == 'B') ||
                            (entryRoad == 'B' && exitRoad == 'C') ||
                            (entryRoad == 'C' && exitRoad == 'D') ||
                            (entryRoad == 'D' && exitRoad == 'A'));

        // The remaining case is left turn

        if (isStraight)
        {
            // Simple linear interpolation for straight movement
            vehicle->x = centerX + (vehicle->endX - centerX) * turnProgress;
            vehicle->y = centerY + (vehicle->endY - centerY) * turnProgress;
        }
        else if (isRightTurn)
        {
            // Right turn calculation
            if (entryRoad == 'A') // A to B
            {
                vehicle->x = centerX + LANE_WIDTH * (exitLaneNum - 1) * turnProgress;
                vehicle->y = centerY - (centerY - vehicle->startY) * (1 - turnProgress);
            }
            else if (entryRoad == 'B') // B to C
            {
                vehicle->x = centerX - (centerX - vehicle->endX) * turnProgress;
                vehicle->y = centerY + LANE_WIDTH * (exitLaneNum - 1) * turnProgress;
            }
            else if (entryRoad == 'C') // C to D
            {
                vehicle->x = centerX - LANE_WIDTH * (exitLaneNum - 1) * turnProgress;
                vehicle->y = centerY + (centerY - vehicle->endY) * turnProgress;
            }
            else if (entryRoad == 'D') // D to A
            {
                vehicle->x = centerX + (centerX - vehicle->endX) * turnProgress;
                vehicle->y = centerY - LANE_WIDTH * (exitLaneNum - 1) * turnProgress;
            }
        }
        else
        {
            // Left turn calculation
            if (entryRoad == 'A') // A to D
            {
                vehicle->x = centerX - LANE_WIDTH * (exitLaneNum - 1) * turnProgress;
                vehicle->y = centerY + (centerY - vehicle->startY) * turnProgress;
            }
            else if (entryRoad == 'B') // B to A
            {
                vehicle->x = centerX - (centerX - vehicle->endX) * turnProgress;
                vehicle->y = centerY - LANE_WIDTH * (exitLaneNum - 1) * turnProgress;
            }
            else if (entryRoad == 'C') // C to B
            {
                vehicle->x = centerX + LANE_WIDTH * (exitLaneNum - 1) * turnProgress;
                vehicle->y = centerY - (centerY - vehicle->endY) * turnProgress;
            }
            else if (entryRoad == 'D') // D to C
            {
                vehicle->x = centerX + (centerX - vehicle->endX) * turnProgress;
                vehicle->y = centerY + LANE_WIDTH * (exitLaneNum - 1) * turnProgress;
            }
        }
    }
    else
    {
        // Leaving intersection (last 40% of journey)
        float adjustedProgress = (vehicle->progress - 0.6f) / 0.4f;
        vehicle->x = centerX + (vehicle->endX - centerX) * adjustedProgress;
        vehicle->y = centerY + (vehicle->endY - centerY) * adjustedProgress;
    }
}