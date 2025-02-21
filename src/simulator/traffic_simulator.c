#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define VEHICLE_WIDTH 60
#define VEHICLE_HEIGHT 35
#define LANE_SPACING 100
#define INTERSECTION_X 960
#define LIGHT_CHANGE_INTERVAL 500

typedef enum
{
    RED,
    GREEN
} TrafficLight;

typedef struct Vehicle
{
    int lane;
    float xPos, yPos;
    bool turning;
    struct Vehicle *next;
} Vehicle;

typedef struct
{
    Vehicle *front;
    Vehicle *rear;
    int size;
} Queue;

void initQueue(Queue *q)
{
    q->front = q->rear = NULL;
    q->size = 0;
}

void enqueue(Queue *q, int lane, float x, float y, bool turning)
{
    Vehicle *newVehicle = (Vehicle *)malloc(sizeof(Vehicle));
    newVehicle->lane = lane;
    newVehicle->xPos = x;
    newVehicle->yPos = y;
    newVehicle->turning = turning;
    newVehicle->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = newVehicle;
    }
    else
    {
        q->rear->next = newVehicle;
        q->rear = newVehicle;
    }
    q->size++;
}

void dequeue(Queue *q)
{
    if (q->front == NULL)
        return;
    Vehicle *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    q->size--;
}

void drawTrafficLight(SDL_Renderer *renderer, int x, int y, TrafficLight state)
{
    SDL_FRect lightRect = {x, y, 60, 180};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &lightRect);

    SDL_FRect colorRect = {x + 15, y + 15, 30, 30};
    SDL_SetRenderDrawColor(renderer, state == RED ? 255 : 0, state == GREEN ? 255 : 0, 0, 255);
    SDL_RenderFillRect(renderer, &colorRect);
}

void drawVehicles(SDL_Renderer *renderer, Queue *q)
{
    Vehicle *temp = q->front;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    while (temp != NULL)
    {
        SDL_FRect carRect = {temp->xPos, temp->yPos, VEHICLE_WIDTH, VEHICLE_HEIGHT};
        SDL_RenderFillRect(renderer, &carRect);
        temp = temp->next;
    }
}

void runTrafficSimulation()
{
    SDL_Window *window = SDL_CreateWindow("Traffic Simulator", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, "opengl");

    TrafficLight light1 = RED, light2 = GREEN;
    Queue road1, road2;
    initQueue(&road1);
    initQueue(&road2);

    bool running = true;
    SDL_Event event;
    srand(time(NULL));

    int frameCount = 0;
    int lightTimer = 0;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        // Random traffic light change
        if (lightTimer >= LIGHT_CHANGE_INTERVAL)
        {
            light1 = (rand() % 2 == 0) ? GREEN : RED;
            light2 = (light1 == GREEN) ? RED : GREEN;
            lightTimer = 0;
        }

        // Spawn vehicles randomly
        if (frameCount % 100 == 0)
        {
            int lane1 = rand() % 3;
            int lane2 = rand() % 3;
            bool turn1 = (rand() % 2 == 0);
            bool turn2 = (rand() % 2 == 0);
            enqueue(&road1, lane1, 50, lane1 * LANE_SPACING + 300, turn1);
            enqueue(&road2, lane2, lane2 * LANE_SPACING + 700, 50, turn2);
        }

        // Move vehicles
        Vehicle *temp = road1.front;
        while (temp != NULL)
        {
            if (light1 == GREEN || temp->xPos < INTERSECTION_X)
            {
                if (temp->turning && temp->xPos >= INTERSECTION_X - 30)
                {
                    temp->yPos += 3; // Turning down
                }
                else
                {
                    temp->xPos += 3;
                }
            }
            temp = temp->next;
        }

        temp = road2.front;
        while (temp != NULL)
        {
            if (light2 == GREEN || temp->yPos < INTERSECTION_X)
            {
                if (temp->turning && temp->yPos >= INTERSECTION_X - 30)
                {
                    temp->xPos -= 3; // Turning left
                }
                else
                {
                    temp->yPos += 3;
                }
            }
            temp = temp->next;
        }

        // Dequeue vehicles leaving screen
        if (road1.front && road1.front->xPos > WINDOW_WIDTH)
            dequeue(&road1);
        if (road2.front && road2.front->yPos > WINDOW_HEIGHT)
            dequeue(&road2);

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw roads
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_FRect roadRect1 = {0, 200, WINDOW_WIDTH, 300};
        SDL_FRect roadRect2 = {900, 0, 300, WINDOW_HEIGHT};
        SDL_RenderFillRect(renderer, &roadRect1);
        SDL_RenderFillRect(renderer, &roadRect2);

        // Draw traffic lights
        drawTrafficLight(renderer, 1800, 300, light1);
        drawTrafficLight(renderer, 900, 100, light2);

        // Draw vehicles
        drawVehicles(renderer, &road1);
        drawVehicles(renderer, &road2);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        frameCount++;
        lightTimer++;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
