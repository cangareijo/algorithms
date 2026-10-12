/*

ALGORITHM DESCRIPTION: ROTATED ELLIPSE RASTERIZATION (BOUNDING BOX SCANLINE METHOD)

================================================================================
1. INTRODUCTION
================================================================================
The problem of drawing an arbitrary ellipse on a discrete pixel grid (rasterization)
is classic in computer graphics. When the axes of the ellipse are aligned with the
coordinate grid (horizontal and vertical), efficient incremental integer algorithms 
such as the Midpoint Ellipse Algorithm (a variation of Bresenham's line algorithm) 
work perfectly. They use decision variables to evaluate the algebraic error at 
each step and step pixel by pixel along the boundary.

However, when an ellipse is rotated by an arbitrary angle theta, the linear 
symmetry breaks down relative to the x and y axes. The implicit equation develops 
a cross-product term (xy). This cross-product term invalidates the simple pixel 
stepping logic used by classic midpoint algorithms because the transition boundaries 
between octants change dynamically depending on the angle.

To overcome this, this program implements a robust and computationally stable 
method: the Bounding Box Scanline Algorithm using the Implicit Ellipse Equation 
and Coordinate Back-Projection.

================================================================================
2. THE MATHEMATICAL FOUNDATION
================================================================================
An aligned ellipse centered at the origin (0, 0) with a semi-major axis 'a' 
(along the x-axis) and semi-minor axis 'b' (along the y-axis) satisfies the 
standard algebraic equation:

    (x'^2 / a^2) + (y'^2 / b^2) = 1

Any point (x', y') where the left-hand side evaluates to less than or equal to 1.0 
is inside or on the perimeter of the ellipse.

When the ellipse is translated to center (xc, yc) and rotated counterclockwise by 
an angle 'theta', we can determine if a target pixel (x, y) in global window space 
lies within the ellipse by mapping it back into the ellipse's local coordinate system.

Step 1: Translate the global pixel coordinate to the ellipse's center:
    dx = x - xc
    dy = y - yc

Step 2: Rotate the translated point back by 'theta' (clockwise rotation to cancel 
the counterclockwise transformation of the ellipse):
    rx =  dx * cos(theta) + dy * sin(theta)
    ry = -dx * sin(theta) + dy * cos(theta)

Step 3: Evaluate the normalized implicit equation using the local coordinates:
    value = (rx^2 / a^2) + (ry^2 / b^2)

If value <= 1.0, the pixel (x, y) sits inside or on the boundary of the ellipse and 
must be colored.

================================================================================
3. CONSTRAINING THE SEARCH SPACE: THE BOUNDING BOX
================================================================================
Evaluating every pixel on an 800x600 screen for every ellipse is highly 
inefficient (O(N) where N is the total screen area). To optimize performance, the 
algorithm restricts evaluation to the tightest possible Axis-Aligned Bounding Box 
(AABB) enclosing the rotated ellipse.

We can model the rotated ellipse as a parametric curve derived from transformed 
basis vectors. The local horizontal major axis vector is U = [a, 0] and the local 
vertical minor axis vector is V = [0, b]. Rotating these vectors by theta gives 
the directional components of the ellipse in global space:

    ux =  a * cos(theta)
    uy =  a * sin(theta)
    vx = -b * sin(theta)
    vy =  b * cos(theta)

The maximum horizontal extent (half-width) and vertical extent (half-height) of 
the projected ellipse can be found by taking the Euclidean norm of these component 
projections:

    half_width  = sqrt(ux^2 + vx^2)
    half_height = sqrt(uy^2 + vy^2)

Thus, the exact boundary limits of the bounding box are:
    min_x = floor(xc - half_width)
    max_x = ceil(xc + half_width)
    min_y = floor(yc - half_height)
    max_y = ceil(yc + half_height)

================================================================================
4. ALGORITHM STEP-BY-STEP EXECUTION
================================================================================
For each ellipse requested:

1. Sanity Check:
   Verify if the dimensions 'a' and 'b' are greater than zero. If not, abort execution 
   for that instance.

2. Precompute Trigonometric and Inverse Constants:
   Calculate cos(theta) and sin(theta). Compute inverse squares (1.0 / a^2) and 
   (1.0 / b^2) ahead of the loops to replace costly divisions with multiplications 
   inside the per-pixel inner loop execution.

3. Extent Calculation:
   Calculate 'half_width' and 'half_height' to establish the spatial bounds.

4. Clipping to Screen Space:
   Clamp min_x, max_x, min_y, and max_y values so they do not exceed the pixel 
   array boundaries (0 to width-1, 0 to height-1). This protects against out-of-bounds 
   memory writes/segmentation faults.

5. Scanline Iteration:
   Run a nested loop structure. The outer loop steps through rows (y from min_y to max_y), 
   and the inner loop steps through columns (x from min_x to max_x).

6. Transformation and Inclusion Testing:
   For every pixel inside the clipped bounding box:
     a. Calculate relative delta offset (dx, dy).
     b. Perform the inverse rotation matrix multiplication to derive local (rx, ry).
     c. Calculate the geometric polynomial value.
     d. If value <= 1.0, update the linear pixel buffer memory location at index 
        [y * buffer_width + x] with the designated hex color code.

================================================================================
5. TRADE-OFFS AND ADVANCED OPTIMIZATIONS
================================================================================
Pros of this approach:
- Mathematical Simplicity: Highly readable, easy to maintain, and mathematically 
  bulletproof across all variations of angles, scale scales, and eccentricities.
- Robustness: Avoids topological gaps, missing pixels, or overlapping issues 
  inherent in raw step-based parametric drawing methods.

Cons of this approach:
- Redundant Evaluations: The algorithm visits pixels inside the bounding box corners 
  that fall outside the ellipse equation threshold.
- Performance Scale: For massive ellipses, floating-point math evaluations per pixel 
  impact performance.

Alternative Optimizations for high-performance contexts:
- Edge Walking (Incremental Scanline): Instead of testing all bounding box points, 
  you can solve the implicit quadratic equation directly for 'x' at each row 'y' 
  to discover the precise entry and exit pixel boundaries, turning the inner loop 
  into a pure horizontal span fill memory operation (memset).

*/

#include <SDL3/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    int width;
    int height;
    uint32_t *pixels;
} PixelBuffer;

void draw_ellipse(PixelBuffer buffer, int xc, int yc, int a, int b, double angle, uint32_t color) {
    if (a <= 0 || b <= 0) return;

    double cos_a = cos(angle);
    double sin_a = sin(angle);

    double ux = a * cos_a;
    double uy = a * sin_a;
    double vx = b * (-sin_a);
    double vy = b * cos_a;

    double half_w = sqrt(ux * ux + vx * vx);
    double half_h = sqrt(uy * uy + vy * vy);

    int min_x = (int)floor(xc - half_w);
    int max_x = (int)ceil(xc + half_w);
    int min_y = (int)floor(yc - half_h);
    int max_y = (int)ceil(yc + half_h);

    if (min_x < 0) min_x = 0;
    if (max_x >= buffer.width) max_x = buffer.width - 1;
    if (min_y < 0) min_y = 0;
    if (max_y >= buffer.height) max_y = buffer.height - 1;

    double inv_a2 = 1.0 / (a * a);
    double inv_b2 = 1.0 / (b * b);

    for (int y = min_y; y <= max_y; ++y) {
        double dy = y - yc;
        for (int x = min_x; x <= max_x; ++x) {
            double dx = x - xc;

            double rx = dx * cos_a + dy * sin_a;
            double ry = -dx * sin_a + dy * cos_a;

            double value = (rx * rx) * inv_a2 + (ry * ry) * inv_b2;

            if (value <= 1.0) {
                buffer.pixels[y * buffer.width + x] = color;
            }
        }
    }
}

int main(int, char *[]) {
    constexpr int width = 800;
    constexpr int height = 600;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Rotated Ellipses", width, height, 0);
    if (!window) {
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    uint32_t *pixels = malloc(width * height * sizeof(uint32_t));
    PixelBuffer buffer = { .width = width, .height = height, .pixels = pixels };

    for (int i = 0; i < width * height; ++i) {
        pixels[i] = 00000000;
    }

    srand((unsigned int)time(NULL));
    for (int i = 0; i < 20; ++i) {
        int xc = rand() % width;
        int yc = rand() % height;
        int a = 30 + rand() % 100;
        int b = 10 + rand() % 50;
        double angle = (rand() % 360) * (M_PI / 180.0);
        uint32_t color = (0xFF << 24) | ((rand() % 256) << 16) | ((rand() % 256) << 8) | (rand() % 256);

        draw_ellipse(buffer, xc, yc, a, b, angle, color);
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
