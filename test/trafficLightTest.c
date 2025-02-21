#include <SDL3/SDL.h>
#include <stdbool.h>

#define WINDOW_WIDTH 200
#define WINDOW_HEIGHT 400
#define LIGHT_RADIUS 40

typedef enum
{
    RED,
    GREEN,
    YELLOW
} LightState;

void drawCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    for (int w = -radius; w < radius; w++)
    {
        for (int h = -radius; h < radius; h++)
        {
            if (w * w + h * h <= radius * radius)
            {
                SDL_RenderPoint(renderer, x + w, y + h);
            }
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Traffic Light", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    bool running = true;
    SDL_Event event;
    LightState state = RED;
    Uint32 lastSwitchTime = SDL_GetTicks();
    const int switchInterval = 2000; // 2 seconds

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastSwitchTime > switchInterval)
        {
            state = (state == RED) ? GREEN : (state == GREEN) ? YELLOW
                                                              : RED;
            lastSwitchTime = currentTime;
        }

        // Clear screen (black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw traffic light box
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_FRect lightBox = {60, 50, 80, 260};
        SDL_RenderFillRect(renderer, &lightBox);

        // Draw lights
        drawCircle(renderer, 100, 100, LIGHT_RADIUS);
        drawCircle(renderer, 100, 200, LIGHT_RADIUS);
        drawCircle(renderer, 100, 300, LIGHT_RADIUS);

        // Set colors based on state
        SDL_SetRenderDrawColor(renderer, state == RED ? 255 : 50, 0, 0, 255);
        drawCircle(renderer, 100, 100, LIGHT_RADIUS);

        SDL_SetRenderDrawColor(renderer, state == YELLOW ? 255 : 50, state == YELLOW ? 255 : 50, 0, 255);
        drawCircle(renderer, 100, 200, LIGHT_RADIUS);

        SDL_SetRenderDrawColor(renderer, 0, state == GREEN ? 255 : 50, 0, 255);
        drawCircle(renderer, 100, 300, LIGHT_RADIUS);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
