#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void draw_line(uint32_t* pixels, int32_t width, int32_t height, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color) {
  if (pixels == NULL) return;

  int32_t dx = abs(x1 - x0);
  int32_t dy = abs(y1 - y0);

  int32_t sx = (x0 < x1) ? 1 : -1;
  int32_t sy = (y0 < y1) ? 1 : -1;

  int32_t err = dx - dy;

  while (true) {
    if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) {
      pixels[y0 * width + x0] = color;
    }

    if (x0 == x1 && y0 == y1) break;

    int32_t e2 = 2 * err;

    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

constexpr int32_t SCREEN_WIDTH = 800;
constexpr int32_t SCREEN_HEIGHT = 600;

bool draw_pixel_array(SDL_Renderer* renderer, SDL_Texture* texture, const uint32_t* pixels, int32_t width) {
  if (renderer == NULL || texture == NULL || pixels == NULL) {
    return false;
  }
  int32_t pitch = width * sizeof(uint32_t);

  if (!SDL_UpdateTexture(texture, NULL, (const uint8_t*)pixels, pitch)) {
    SDL_Log("Failed to update texture: %s", SDL_GetError());
    return false;
  }

  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
  return true;
}

int main(void) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL could not initialize: %s", SDL_GetError());
    return 1;
  }

  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;

  if (!SDL_CreateWindowAndRenderer("C23 Pixel Array Renderer", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
    SDL_Log("Window/Renderer creation failed: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Texture* texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
  );

  if (texture == NULL) {
    SDL_Log("Texture creation failed: %s", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  uint32_t* pixel_buffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
  if (pixel_buffer == NULL) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  uint32_t clear_color = 0xFF000000;
  uint32_t line_color = 0xFFFFFFFF;

  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
    pixel_buffer[i] = clear_color;
  }

  draw_line(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, line_color);
  draw_line(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1, 0, line_color);

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    draw_pixel_array(renderer, texture, pixel_buffer, SCREEN_WIDTH);
    SDL_Delay(16);
  }

  free(pixel_buffer);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
