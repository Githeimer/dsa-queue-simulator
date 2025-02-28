#include "vehicleQueue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "animation.h"
#include "math.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900
#define ROAD_WIDTH 180  // Each road's total width (3 lanes x 60 width)
#define LANE_WIDTH 60   // Width of each lane
#define VEHICLE_SIZE 30 // Vehicle size (width and height)
#define M_PI 3.14159265358979323846

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

void checkQueue(LaneQueue *AL2Queue, LaneQueue *BL2Queue, LaneQueue *CL2Queue, LaneQueue *DL2Queue,
                int id, const char *entryLane, const char *exitLane, const char *direction,
                SDL_Renderer *renderer)
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
        printf("Enqueued vehicle with ID %d to CL2\n", id); // Fixed: was printing DL2
    }
    else if (strcmp(entryLane, "DL2") == 0)
    {
        enqueueVehicle(DL2Queue, id, entryLane, exitLane, direction, renderer);
        printf("Enqueued vehicle with ID %d to DL2\n", id);
    }
    else
    {
        printf("No matching entry lane for vehicle ID %d: %s\n", id, entryLane);
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
    // Base vehicle colors based on direction
    SDL_Color baseColor;
    if (strcmp(direction, "N") == 0)
    {
        baseColor = (SDL_Color){143, 0, 255, 255}; // Purple
    }
    else if (strcmp(direction, "E") == 0)
    {
        baseColor = (SDL_Color){33, 79, 198, 255}; // Blue
    }
    else if (strcmp(direction, "S") == 0)
    {
        baseColor = (SDL_Color){0, 0, 255, 255}; // Dark Blue
    }
    else if (strcmp(direction, "W") == 0)
    {
        baseColor = (SDL_Color){255, 127, 0, 255}; // Orange
    }
    else
    {
        baseColor = (SDL_Color){8, 133, 161, 255}; // Teal
    }

    // Calculate brighter color for highlights
    SDL_Color lightColor = {
        (Uint8)SDL_min(255, baseColor.r + 40),
        (Uint8)SDL_min(255, baseColor.g + 40),
        (Uint8)SDL_min(255, baseColor.b + 40),
        255};

    // Calculate darker color for shadows
    SDL_Color darkColor = {
        (Uint8)SDL_max(0, baseColor.r - 40),
        (Uint8)SDL_max(0, baseColor.g - 40),
        (Uint8)SDL_max(0, baseColor.b - 40),
        255};

    // Car body dimensions
    float carWidth = VEHICLE_SIZE;
    float carLength = VEHICLE_SIZE * 1.5;
    float carHeight = VEHICLE_SIZE * 0.7; // For 3D effect

    // Rotate dimensions based on direction
    float width, length;
    if (strcmp(direction, "N") == 0 || strcmp(direction, "S") == 0)
    {
        width = carWidth;
        length = carLength;
    }
    else
    {
        width = carLength;
        length = carWidth;
    }

    // Draw shadow (offset slightly)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
    SDL_FRect shadowRect = {
        x - width / 2 + 3,
        y - length / 2 + 3,
        width,
        length};
    SDL_RenderFillRect(renderer, &shadowRect);

    // Draw car body
    SDL_SetRenderDrawColor(renderer, baseColor.r, baseColor.g, baseColor.b, baseColor.a);
    SDL_FRect carBody = {
        x - width / 2,
        y - length / 2,
        width,
        length};
    SDL_RenderFillRect(renderer, &carBody);

    // Draw car top/cabin (smaller rectangle in the middle)
    if (strcmp(direction, "N") == 0)
    {
        // Draw top section for north-facing car
        SDL_SetRenderDrawColor(renderer, lightColor.r, lightColor.g, lightColor.b, lightColor.a);
        SDL_FRect carTop = {
            x - width / 2 + width * 0.2,
            y - length / 2 + length * 0.2,
            width * 0.6,
            length * 0.4};
        SDL_RenderFillRect(renderer, &carTop);

        // Draw windshield
        SDL_SetRenderDrawColor(renderer, 200, 230, 255, 255); // Light blue for glass
        SDL_FRect windshield = {
            x - width / 2 + width * 0.25,
            y - length / 2 + length * 0.2,
            width * 0.5,
            length * 0.1};
        SDL_RenderFillRect(renderer, &windshield);

        // Draw headlights
        SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255); // Yellow-white
        SDL_FRect headlightLeft = {
            x - width / 2 + width * 0.2,
            y - length / 2 + length * 0.05,
            width * 0.15,
            length * 0.05};
        SDL_FRect headlightRight = {
            x - width / 2 + width * 0.65,
            y - length / 2 + length * 0.05,
            width * 0.15,
            length * 0.05};
        SDL_RenderFillRect(renderer, &headlightLeft);
        SDL_RenderFillRect(renderer, &headlightRight);
    }
    else if (strcmp(direction, "S") == 0)
    {
        // Draw top section for south-facing car
        SDL_SetRenderDrawColor(renderer, lightColor.r, lightColor.g, lightColor.b, lightColor.a);
        SDL_FRect carTop = {
            x - width / 2 + width * 0.2,
            y - length / 2 + length * 0.4,
            width * 0.6,
            length * 0.4};
        SDL_RenderFillRect(renderer, &carTop);

        // Draw windshield
        SDL_SetRenderDrawColor(renderer, 200, 230, 255, 255);
        SDL_FRect windshield = {
            x - width / 2 + width * 0.25,
            y - length / 2 + length * 0.7,
            width * 0.5,
            length * 0.1};
        SDL_RenderFillRect(renderer, &windshield);

        // Draw tail lights
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255); // Red
        SDL_FRect tailLightLeft = {
            x - width / 2 + width * 0.2,
            y - length / 2 + length * 0.9,
            width * 0.15,
            length * 0.05};
        SDL_FRect tailLightRight = {
            x - width / 2 + width * 0.65,
            y - length / 2 + length * 0.9,
            width * 0.15,
            length * 0.05};
        SDL_RenderFillRect(renderer, &tailLightLeft);
        SDL_RenderFillRect(renderer, &tailLightRight);
    }
    else if (strcmp(direction, "E") == 0)
    {
        // Draw top section for east-facing car
        SDL_SetRenderDrawColor(renderer, lightColor.r, lightColor.g, lightColor.b, lightColor.a);
        SDL_FRect carTop = {
            x - width / 2 + width * 0.4,
            y - length / 2 + length * 0.2,
            width * 0.4,
            length * 0.6};
        SDL_RenderFillRect(renderer, &carTop);

        // Draw windshield
        SDL_SetRenderDrawColor(renderer, 200, 230, 255, 255);
        SDL_FRect windshield = {
            x - width / 2 + width * 0.7,
            y - length / 2 + length * 0.25,
            width * 0.1,
            length * 0.5};
        SDL_RenderFillRect(renderer, &windshield);

        // Draw headlights
        SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255); // Yellow-white
        SDL_FRect headlight = {
            x - width / 2 + width * 0.9,
            y - length / 2 + length * 0.25,
            width * 0.05,
            length * 0.15};
        SDL_FRect headlight2 = {
            x - width / 2 + width * 0.9,
            y - length / 2 + length * 0.6,
            width * 0.05,
            length * 0.15};
        SDL_RenderFillRect(renderer, &headlight);
        SDL_RenderFillRect(renderer, &headlight2);
    }
    else if (strcmp(direction, "W") == 0)
    {
        // Draw top section for west-facing car
        SDL_SetRenderDrawColor(renderer, lightColor.r, lightColor.g, lightColor.b, lightColor.a);
        SDL_FRect carTop = {
            x - width / 2 + width * 0.2,
            y - length / 2 + length * 0.2,
            width * 0.4,
            length * 0.6};
        SDL_RenderFillRect(renderer, &carTop);

        // Draw windshield
        SDL_SetRenderDrawColor(renderer, 200, 230, 255, 255);
        SDL_FRect windshield = {
            x - width / 2 + width * 0.2,
            y - length / 2 + length * 0.25,
            width * 0.1,
            length * 0.5};
        SDL_RenderFillRect(renderer, &windshield);

        // Draw tail lights
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255); // Red
        SDL_FRect tailLight = {
            x - width / 2 + width * 0.05,
            y - length / 2 + length * 0.25,
            width * 0.05,
            length * 0.15};
        SDL_FRect tailLight2 = {
            x - width / 2 + width * 0.05,
            y - length / 2 + length * 0.6,
            width * 0.05,
            length * 0.15};
        SDL_RenderFillRect(renderer, &tailLight);
        SDL_RenderFillRect(renderer, &tailLight2);
    }

    // Draw wheels
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Dark gray/black

    float wheelSize = VEHICLE_SIZE * 0.25;
    float wheelOffset = VEHICLE_SIZE * 0.3;

    if (strcmp(direction, "N") == 0 || strcmp(direction, "S") == 0)
    {
        // Front-left wheel
        SDL_FRect wheelFL = {
            x - width / 2 - wheelSize / 3,
            y - length / 2 + wheelOffset,
            wheelSize,
            wheelSize};
        SDL_RenderFillRect(renderer, &wheelFL);

        // Front-right wheel
        SDL_FRect wheelFR = {
            x + width / 2 - wheelSize + wheelSize / 3,
            y - length / 2 + wheelOffset,
            wheelSize,
            wheelSize};
        SDL_RenderFillRect(renderer, &wheelFR);

        // Rear-left wheel
        SDL_FRect wheelRL = {
            x - width / 2 - wheelSize / 3,
            y + length / 2 - wheelOffset - wheelSize,
            wheelSize,
            wheelSize};
        SDL_RenderFillRect(renderer, &wheelRL);

        // Rear-right wheel
        SDL_FRect wheelRR = {
            x + width / 2 - wheelSize + wheelSize / 3,
            y + length / 2 - wheelOffset - wheelSize,
            wheelSize,
            wheelSize};
        SDL_RenderFillRect(renderer, &wheelRR);
    }
    else
    {
        // Left-front wheel
        SDL_FRect wheelLF = {
            x - width / 2 + wheelOffset,
            y - length / 2 - wheelSize / 3,
            wheelSize,
            wheelSize};
        SDL_RenderFillRect(renderer, &wheelLF);

        // Right-front wheel
        SDL_FRect wheelRF = {
            x + width / 2 - wheelOffset - wheelSize,
            y - length / 2 - wheelSize / 3,
            wheelSize,
            wheelSize};
        SDL_RenderFillRect(renderer, &wheelRF);

        // Left-rear wheel
        SDL_FRect wheelLR = {
            x - width / 2 + wheelOffset,
            y + length / 2 - wheelSize + wheelSize / 3,
            wheelSize,
            wheelSize};
        SDL_RenderFillRect(renderer, &wheelLR);

        // Right-rear wheel
        SDL_FRect wheelRR = {
            x + width / 2 - wheelOffset - wheelSize,
            y + length / 2 - wheelSize + wheelSize / 3,
            wheelSize,
            wheelSize};
        SDL_RenderFillRect(renderer, &wheelRR);
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
        // Store next pointer since current might be freed
        Vehicle *next = current->next;

        if (!current->active)
        {
            // Remove inactive vehicles from the queue
            if (prev == NULL) // If it's the front vehicle
            {
                queue->front = current->next;
                if (queue->front == NULL) // If queue becomes empty
                    queue->rear = NULL;
            }
            else
            {
                prev->next = current->next;
                if (current == queue->rear) // If it's the rear vehicle
                    queue->rear = prev;
            }

            queue->size--;
            free(current);
            current = next;
            continue;
        }

        // Check if vehicle is approaching the intersection entrance
        bool atIntersectionEntrance = (current->progress >= 0.25f && current->progress < 0.4f);

        // Check if vehicle has entered the intersection
        bool hasEnteredIntersection = (current->progress >= 0.4f);

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

        // Only stop at red light if at intersection entrance and NOT already in the intersection
        if (atIntersectionEntrance && !canMoveThrough && !hasEnteredIntersection)
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

                // We'll remove it in the next iteration
            }
            else
            {
                // Update position based on progress
                updateVehiclePositionBasedOnPath(current, centerX, centerY);
            }
        }

        // Move to next vehicle
        prev = current;
        current = next;
    }
}

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