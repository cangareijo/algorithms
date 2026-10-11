#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint32_t* pixels;
    int32_t width;
    int32_t height;
} PixelArray;

static inline void set_pixel(const PixelArray* canvas, int32_t x, int32_t y, uint32_t color) {
    if (x >= 0 && x < canvas->width && y >= 0 && y < canvas->height) {
        canvas->pixels[y * canvas->width + x] = color;
    }
}

void draw_ellipse(const PixelArray* canvas, int32_t xc, int32_t yc, int32_t a, int32_t b, uint32_t color) {

    if (a <= 0 || b <= 0) return;

    int32_t x = 0;
    int32_t y = b;

    int32_t a2 = a * a;
    int32_t b2 = b * b;
    int32_t two_a2 = 2 * a2;
    int32_t two_b2 = 2 * b2;

    int32_t px = 0;
    int32_t py = two_a2 * y;

    int32_t p = (int32_t)(b2 - (a2 * b) + (0.25f * a2) + 0.5f);

    while (px < py) {

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

    p = (int32_t)(b2 * (x + 0.5f) * (x + 0.5f) + a2 * (y - 1) * (y - 1) - a2 * b2 + 0.5f);

    while (y >= 0) {

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

    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING,
                                     window_width, window_height);
    if (texture == nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

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

        memset(canvas.pixels, 0x1A, window_width * window_height * sizeof(uint32_t));

        draw_ellipse(&canvas, 400, 300, 250, 150, 0xFF00FFFF);

        draw_ellipse(&canvas, 400, 300, 100, 200, 0xFFFF00FF);

        draw_ellipse(&canvas, 400, 300, 50, 50, 0xFFFFCC00);

        draw_ellipse(&canvas, 100, 100, 80, 40, 0xFF00FF00);

        SDL_UpdateTexture(texture, nullptr, canvas.pixels, window_width * sizeof(uint32_t));

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    free(pixel_buffer);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
