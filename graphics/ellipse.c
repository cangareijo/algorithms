#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// A simple structure to represent our target pixel canvas
typedef struct {
    uint32_t* pixels;
    int32_t width;
    int32_t height;
} PixelArray;

// Helper function to safely set a pixel color with bounds checking
static inline void set_pixel(const PixelArray* canvas, int32_t x, int32_t y, uint32_t color) {
    if (x >= 0 && x < canvas->width && y >= 0 && y < canvas->height) {
        canvas->pixels[y * canvas->width + x] = color;
    }
}

/**
 * Draws an ellipse on a pixel array using the Midpoint Ellipse Algorithm.
 * Structured cleanly using standard C23 patterns.
 */
void draw_ellipse(const PixelArray* canvas, int32_t xc, int32_t yc, int32_t a, int32_t b, uint32_t color) {
    // Avoid division by zero or negative dimensions
    if (a <= 0 || b <= 0) return;

    int32_t x = 0;
    int32_t y = b;

    // Intermediate terms optimized out to avoid repeated multiplication
    int32_t a2 = a * a;
    int32_t b2 = b * b;
    int32_t two_a2 = 2 * a2;
    int32_t two_b2 = 2 * b2;
    
    int32_t px = 0;
    int32_t py = two_a2 * y;

    // --- Region 1 ---
    // Initial decision parameter for Region 1
    int32_t p = (int32_t)(b2 - (a2 * b) + (0.25f * a2) + 0.5f); 
    
    while (px < py) {
        // Plot symmetric points in all 4 quadrants
        set_pixel(canvas, xc + x, yc + y, color);
        set_pixel(canvas, xc - x, yc + y, color);
        set_pixel(canvas, xc + x, yc - y, color);
        set_pixel(canvas, xc - x, yc - y, color);

        x++;
        px += two_b2;
        
        if (p < 0) {
            p += b2 + px;
        } else {
            y--;
            py -= two_a2;
            p += b2 + px - py;
        }
    }

    // --- Region 2 ---
    // Decision parameter for Region 2 based on the last point of Region 1
    p = (int32_t)(b2 * (x + 0.5f) * (x + 0.5f) + a2 * (y - 1) * (y - 1) - a2 * b2 + 0.5f);

    while (y >= 0) {
        // Plot symmetric points in all 4 quadrants
        set_pixel(canvas, xc + x, yc + y, color);
        set_pixel(canvas, xc - x, yc + y, color);
        set_pixel(canvas, xc + x, yc - y, color);
        set_pixel(canvas, xc - x, yc - y, color);

        y--;
        py -= two_a2;

        if (p > 0) {
            p += a2 - py;
        } else {
            x++;
            px += two_b2;
            p += a2 - py + px;
        }
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    // Modern C23: Use nullptr instead of NULL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return -1;
    }

    constexpr int32_t window_width = 800;
    constexpr int32_t window_height = 600;

    auto window = SDL_CreateWindow("Midpoint Ellipse Algorithm (C23)", 
                                   window_width, window_height, 0);
    if (window == nullptr) {
        SDL_Quit();
        return -1;
    }

    auto renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Create a streaming texture so we can map a raw pixel buffer directly
    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                                     SDL_TEXTUREACCESS_STREAMING, 
                                     window_width, window_height);
    if (texture == nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Allocate memory for our canvas buffer
    uint32_t* pixel_buffer = malloc(window_width * window_height * sizeof(uint32_t));
    PixelArray canvas = { .pixels = pixel_buffer, .width = window_width, .height = window_height };

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // 1. Clear the pixel canvas buffer (Dark Navy Background)
        memset(canvas.pixels, 0x1A, window_width * window_height * sizeof(uint32_t));

        // 2. Draw various test ellipses on our pixel array
        // Cyan Ellipse in center
        draw_ellipse(&canvas, 400, 300, 250, 150, 0xFF00FFFF); 
        // Magenta Ellipse
        draw_ellipse(&canvas, 400, 300, 100, 200, 0xFFFF00FF); 
        // Yellow concentric ring
        draw_ellipse(&canvas, 400, 300, 50, 50, 0xFFFFCC00);   
        // Corner checks
        draw_ellipse(&canvas, 100, 100, 80, 40, 0xFF00FF00);   

        // 3. Push pixel array updates to the GPU texture
        SDL_UpdateTexture(texture, nullptr, canvas.pixels, window_width * sizeof(uint32_t));

        // 4. Render to the window
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Target ~60 FPS
    }

    // Cleanup resources safely
    free(pixel_buffer);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
