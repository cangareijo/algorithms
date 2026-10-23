#define SDL_MAIN_USE_CALLBACKS 1 // Tell SDL3 to use the modern callback structure
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// Struct to hold our global application state
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool quit;
} AppState;

// Performance-optimized circle fill that avoids pixel gaps
void SDL_RenderFillCircle(SDL_Renderer* renderer, float centerX, float centerY, float radius) {
    float r2 = radius * radius;
    
    // Calculate vertical integer bounding boundaries for screen pixels
    int startY = (int)(centerY - radius);
    int endY = (int)(centerY + radius);

    // Loop through rows sequentially inside the bounding box
    for (int y = startY; y <= endY; y++) {
        float dy = (float)y - centerY;
        float dy2 = dy * dy;
        
        // Use the Pythagorean theorem to calculate the width of the circle span at this height
        if (dy2 <= r2) {
            float width = SDL_sqrtf(r2 - dy2);
            float x1 = centerX - width;
            float x2 = centerX + width;
            
            // Draw a single, solid subpixel-perfect line segment
            SDL_RenderLine(renderer, x1, (float)y, x2, (float)y);
        }
    }
}

// 1. Called once at application startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    // Suppress unused parameter warnings explicitly using C23 attributes
    [[maybe_unused]] int unused_argc = argc;
    [[maybe_unused]] char** unused_argv = argv;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Allocate memory for our app state
    AppState* state = SDL_calloc(1, sizeof(AppState));
    if (!state) {
        return SDL_APP_FAILURE;
    }
    *appstate = state;

    if (!SDL_CreateWindowAndRenderer("Optimized C23 SDL3 Circle", 800, 600, 0, &state->window, &state->renderer)) {
        SDL_Log("Window/Renderer creation failed! SDL_Error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

// 2. Called once per frame to handle rendering logic
SDL_AppResult SDL_AppIterate(void* appstate) {
    AppState* state = (AppState*)appstate;

    // Clear background
    SDL_SetRenderDrawColor(state->renderer, 20, 20, 40, 255);
    SDL_RenderClear(state->renderer);

    // Draw a perfectly filled orange circle
    SDL_SetRenderDrawColor(state->renderer, 255, 140, 0, 255);
    SDL_RenderFillCircle(state->renderer, 400.0f, 300.0f, 150.0f);

    SDL_RenderPresent(state->renderer);

    return state->quit ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}

// 3. Called automatically whenever an event occurs
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    AppState* state = (AppState*)appstate;
    
    if (event->type == SDL_EVENT_QUIT) {
        state->quit = true;
    }
    return SDL_APP_CONTINUE;
}

// 4. Called automatically right before the application terminates
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    if (appstate) {
        AppState* state = (AppState*)appstate;
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_free(state);
    }
    SDL_Quit();
    SDL_Log("Application exited clean with code %d\n", result);
}
