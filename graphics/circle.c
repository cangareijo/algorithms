#include <SDL3/SDL.h>
#include <stdbool.h>

// Compile-time application configurations using modern C23 constexpr
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int TARGET_FPS = 60;
constexpr Uint64 FRAME_DELAY_NS = 1000000000 / TARGET_FPS; // Total nanosecond budget per single frame

/**
 * 8-Way Symmetry Processing Function
 * -----------------------------------
 * Circles are mathematically symmetrical across 8 distinct 45-degree slices (octants).
 * This means we only need to calculate the coordinates for 1 octant (from 0 to 45 degrees).
 * The remaining 7 points are generated for free by mirroring the X and Y coordinates.
 */
void draw_circle_octants(SDL_Renderer *renderer, float cx, float cy, float x, float y) {
  // We construct an array of 8 points using our single (x, y) offset offset from center (cx, cy)
  const SDL_FPoint points[] = {
    { cx + x, cy + y }, // Octant 1: Standard coordinate (+x, +y)
    { cx - x, cy + y }, // Octant 2: Mirror across the Y-axis (-x, +y)
    { cx + x, cy - y }, // Octant 3: Mirror across the X-axis (+x, -y)
    { cx - x, cy - y }, // Octant 4: Mirror across both axes (-x, -y)
    { cx + y, cy + x }, // Octant 5: Swap X/Y axes (+y, +x)
    { cx - y, cy + x }, // Octant 6: Swap X/Y and mirror Y (-y, +x)
    { cx + y, cy - x }, // Octant 7: Swap X/Y and mirror X (+y, -x)
    { cx - y, cy - x }  // Octant 8: Swap X/Y and mirror both (-y, -x)
  };

  // SDL3 Optimization: Instead of 8 individual graphics cards commands, we batch 
  // all 8 points together into a single GPU primitive operation.
  SDL_RenderPoints(renderer, points, 8);
}

/**
 * Midpoint (Bresenham's) Circle Algorithm
 * ---------------------------------------
 * Traces a circle pixel-by-pixel using ONLY integer addition, subtraction, and bitwise shifts.
 * This avoids expensive trigonometric functions (sin/cos) and slow floating-point math.
 */
void draw_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
  int x = 0;          // Start tracing at the very top of the circle (0 degrees)
  int y = radius;     // The initial height is the full length of the radius
  
  // The 'Decision Parameter' (d) keeps track of how close our current pixel path is 
  // to the real geometric path. The initial mathematical equation is: d = 3 - (2 * radius).
  // Optimization: (radius << 1) multiplies the integer by 2 using a fast bitwise shift left.
  int d = 3 - (radius << 1); 

  // Safely pre-cast the center point into floats outside the loop to optimize operations
  const float fcx = (float)cx;
  const float fcy = (float)cy;

  // Plot the initial 4 cardinal boundary points (Top, Bottom, Left, Right)
  draw_circle_octants(renderer, fcx, fcy, (float)x, (float)y);

  // Loop runs through exactly 1 octant (45 degrees), ending when X crosses or equals Y
  while (y >= x) {
    x++; // We always advance 1 pixel to the right on every single step

    // Check if our path has stepped outside the boundary of the ideal circle
    if (d > 0) {
      // If d > 0, the midpoint is OUTSIDE the perimeter.
      // We must move down diagonally (South-East), so we decrement Y.
      y--;
      
      // Update decision parameter for the next step. 
      // Math: d = d + 4 * (x - y) + 10. 
      // Optimization: Using << 2 shifts the bits left to perform multiplication by 4.
      d += ((x - y) << 2) + 10;
    } else {
      // If d <= 0, the midpoint is INSIDE or ON the perimeter line.
      // We stay on the same horizontal row (East), so Y remains unchanged.
      
      // Update decision parameter for the next step.
      // Math: d = d + 4 * x + 6.
      d += (x << 2) + 6;
    }
    
    // Draw the newly calculated point alongside its 7 mirrored twins
    draw_circle_octants(renderer, fcx, fcy, (float)x, (float)y);
  }
}

int main(int argc, char *argv[]) {
  // Explicitly silence compiler warnings regarding unused argument parameters
  (void)argc;
  (void)argv;

  // Modern C23 feature: nullptr is now a native keywords, replacing the old NULL macro
  SDL_Window* window = nullptr;
  SDL_Renderer *renderer = nullptr;

  // Streamlined SDL3 allocation: Creates both our program window and GPU context concurrently
  if (!SDL_CreateWindowAndRenderer("Didactic C23 Circle Renderer", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
    SDL_Log("Failed to initialize SDL3 window/renderer: %s", SDL_GetError());
    return 1;
  }

  bool quit = false;
  SDL_Event e = {}; // Modern C23 brace syntax safely zeroes out the entire event layout context
  
  float pulsing_radius = 50.0f;
  float pulse_direction = 1.0f;

  // Main interactive application run loop
  while (!quit) {
    // Record exactly when the frame tracking starts using high-precision nanoseconds
    Uint64 start_time = SDL_GetTicksNS(); 

    // Handle OS Window Management updates (like closing down the application window)
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) {
        quit = true;
      }
    }

    // --- Simulation Processing Step ---
    // Modify the circle radius up or down slightly every frame to produce an animation
    pulsing_radius += pulse_direction * 1.5f;
    if (pulsing_radius > 250.0f || pulsing_radius < 20.0f) {
      pulse_direction *= -1.0f; // Swap between shrinking and expanding when hitting boundaries
    }

    // --- Render Processing Step ---
    // 1. Set background clearing color to dark charcoal black
    SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x16, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Calculate static center positions
    constexpr int center_x = SCREEN_WIDTH / 2;
    constexpr int center_y = SCREEN_HEIGHT / 2;
    
    // 2. Set draw pipeline color to Neon Cyan and render the outer animated circle
    SDL_SetRenderDrawColor(renderer, 0x00, 0xE5, 0xFF, SDL_ALPHA_OPAQUE);
    draw_circle(renderer, center_x, center_y, (int)pulsing_radius);
    
    // 3. Set draw pipeline color to Neon Magenta and render an inner half-sized copy
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x7F, SDL_ALPHA_OPAQUE);
    draw_circle(renderer, center_x, center_y, (int)pulsing_radius / 2);

    // 4. Swap buffers to physically draw the fully rendered layout to the screen
    SDL_RenderPresent(renderer);

    // --- Framerate Cap & CPU Safety Management ---
    // Measure exactly how many nanoseconds it took to process our entire frame logic
    Uint64 frame_time = SDL_GetTicksNS() - start_time;
    if (frame_time < FRAME_DELAY_NS) {
      // If we finished processing early, sleep the thread to protect CPU usage metrics [1]
      SDL_DelayNS(FRAME_DELAY_NS - frame_time); 
    }
  }

  // Gracefully clean up all allocated structures before program exit
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  return 0;
}
