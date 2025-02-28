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
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for North
    }
    else if (strcmp(direction, "E") == 0)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for East
    }
    else if (strcmp(direction, "S") == 0)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue for South
    }
    else if (strcmp(direction, "W") == 0)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow for West
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Default to red
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
    Vehicle *current = queue->front;
    Vehicle *prev = NULL;

    while (current != NULL)
    {
        if (current->active)
        {
            // Update vehicle position based on progress
            current->progress += 0.002f; // Adjust speed as needed

            if (current->progress >= 1.0f)
            {
                // Vehicle reached destination
                current->active = false;

                // If at front of queue, dequeue it
                if (prev == NULL)
                {
                    dequeueVehicle(queue);
                    current = queue->front;
                    continue;
                }
            }
            else
            {
                // Check if we need to perform lane switching
                // For example, if a vehicle needs to turn, it should start switching lanes
                // before reaching the intersection

                // Calculate intersection center
                int centerX = WINDOW_WIDTH / 2;
                int centerY = WINDOW_HEIGHT / 2;

                // Basic path planning for lane switching - this is a simplified approach
                // For proper lane switching, you would need more complex path planning

                // Get entry road and exit road
                char entryRoad = current->entryLane[0]; // First character (A, B, C, or D)
                char exitRoad = current->exitLane[0];   // First character (A, B, C, or D)

                // Get entry and exit lane numbers
                int entryLaneNum = current->entryLane[2] - '0';
                int exitLaneNum = current->exitLane[2] - '0';

                // Path adjustments based on direction of travel
                float interX = current->x;
                float interY = current->y;

                // If approaching the intersection (between 0.4 and 0.6 progress)
                if (current->progress > 0.4f && current->progress < 0.6f)
                {
                    // Custom path through intersection based on entry/exit roads
                    if ((entryRoad == 'A' && exitRoad == 'C') ||
                        (entryRoad == 'C' && exitRoad == 'A') ||
                        (entryRoad == 'B' && exitRoad == 'D') ||
                        (entryRoad == 'D' && exitRoad == 'B'))
                    {
                        // Straight path - no adjustments needed
                    }
                    else if ((entryRoad == 'A' && exitRoad == 'B') ||
                             (entryRoad == 'B' && exitRoad == 'C') ||
                             (entryRoad == 'C' && exitRoad == 'D') ||
                             (entryRoad == 'D' && exitRoad == 'A'))
                    {
                        // Right turn - adjust path
                        float turnFactor = (current->progress - 0.4f) / 0.2f; // 0 to 1 during the turn

                        if (entryRoad == 'A')
                        {
                            // From A to B (turn right)
                            interX = current->startX + (centerX + LANE_WIDTH * (exitLaneNum - 1) - current->startX) * turnFactor;
                            interY = current->startY + (centerY - current->startY) * turnFactor;
                        }
                        else if (entryRoad == 'B')
                        {
                            // From B to C (turn right)
                            interX = current->startX - (current->startX - centerX) * turnFactor;
                            interY = current->startY + (centerY + LANE_WIDTH * (exitLaneNum - 1) - current->startY) * turnFactor;
                        }
                        else if (entryRoad == 'C')
                        {
                            // From C to D (turn right)
                            interX = current->startX - (current->startX - centerX) * turnFactor;
                            interY = current->startY - (current->startY - centerY) * turnFactor;
                        }
                        else if (entryRoad == 'D')
                        {
                            // From D to A (turn right)
                            interX = current->startX + (centerX - current->startX) * turnFactor;
                            interY = current->startY - (current->startY - centerY) * turnFactor;
                        }

                        current->x = interX;
                        current->y = interY;
                        continue; // Skip the normal interpolation
                    }
                    else if ((entryRoad == 'A' && exitRoad == 'D') ||
                             (entryRoad == 'B' && exitRoad == 'A') ||
                             (entryRoad == 'C' && exitRoad == 'B') ||
                             (entryRoad == 'D' && exitRoad == 'C'))
                    {
                        // Left turn - adjust path
                        float turnFactor = (current->progress - 0.4f) / 0.2f; // 0 to 1 during the turn

                        if (entryRoad == 'A')
                        {
                            // From A to D (turn left)
                            interX = current->startX - (current->startX - centerX) * turnFactor;
                            interY = current->startY + (centerY - current->startY) * turnFactor;
                        }
                        else if (entryRoad == 'B')
                        {
                            // From B to A (turn left)
                            interX = current->startX - (current->startX - centerX) * turnFactor;
                            interY = current->startY - (current->startY - centerY) * turnFactor;
                        }
                        else if (entryRoad == 'C')
                        {
                            // From C to B (turn left)
                            interX = current->startX + (centerX - current->startX) * turnFactor;
                            interY = current->startY - (current->startY - centerY) * turnFactor;
                        }
                        else if (entryRoad == 'D')
                        {
                            // From D to C (turn left)
                            interX = current->startX + (centerX - current->startX) * turnFactor;
                            interY = current->startY + (centerY - current->startY) * turnFactor;
                        }

                        current->x = interX;
                        current->y = interY;
                        continue; // Skip the normal interpolation
                    }
                }

                // Normal linear interpolation for beginning and end of journey
                if (current->progress <= 0.4f || current->progress >= 0.6f)
                {
                    if (current->progress < 0.4f)
                    {
                        // First 40% of journey - go from start to intersection approach
                        float adjustedProgress = current->progress / 0.4f;
                        current->x = current->startX + (centerX - current->startX) * adjustedProgress;
                        current->y = current->startY + (centerY - current->startY) * adjustedProgress;
                    }
                    else
                    {
                        // Last 40% of journey - go from intersection exit to end
                        float adjustedProgress = (current->progress - 0.6f) / 0.4f;
                        current->x = centerX + (current->endX - centerX) * adjustedProgress;
                        current->y = centerY + (current->endY - centerY) * adjustedProgress;
                    }
                }
            }
        }

        prev = current;
        current = current->next;
    }
}
