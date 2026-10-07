#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t Color;

void draw_line(Color *grid, int32_t width, int32_t height, int32_t x0, int32_t y0, int32_t x1, int32_t y1, Color color)
{
  if (!grid) return;
  const int32_t dx = abs(x1 - x0);
  const int32_t dy = abs(y1 - y0);
  const int32_t sx = x0 < x1 ? 1 : -1;
  const int32_t sy = y0 < y1 ? 1 : -1;
  int32_t err = dx - dy;
  while (true) {
    if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) grid[y0 * width + x0] = color;
    if (x0 == x1 && y0 == y1) break;
    const int32_t e2 = 2 * err;
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

bool save_grid_to_bmp(const char *filename, const Color *grid, int32_t width, int32_t height)
{
  if (!filename || !grid || width <= 0 || height <= 0) return false;
  FILE *file = fopen(filename, "xb");
  if (!file) return false;
  const int32_t bytes_per_pixel = 3;
  const int32_t row_stride = width * bytes_per_pixel;
  const int32_t padding_size = (4 - (row_stride % 4)) % 4;
  const int32_t padded_row_stride = row_stride + padding_size;
  const uint32_t header_size = 54;
  const uint32_t pixel_data_size = (uint32_t)padded_row_stride * (uint32_t)height;
  const uint32_t file_size = header_size + pixel_data_size;
  uint8_t file_header[14] = {
    'B', 'M',
    file_size & 0xFF,
    (file_size >> 8) & 0xFF,
    (file_size >> 16) & 0xFF,
    (file_size >> 24) & 0xFF,
    0, 0,
    0, 0,
    header_size & 0xFF,
    (header_size >> 8) & 0xFF,
    (header_size >> 16) & 0xFF,
    (header_size >> 24) & 0xFF
  };
  if (fwrite(file_header, 1, sizeof(file_header), file) != sizeof(file_header)) {
    fclose(file);
    return false;
  }
  const uint32_t dib_size = 40;
  const uint16_t color_planes = 1;
  const uint16_t bits_per_pixel = 24;
  const uint32_t compression = 0;
  uint8_t info_header[40] = {
    dib_size & 0xFF, (dib_size >> 8) & 0xFF, 0, 0,
    width & 0xFF, (width >> 8) & 0xFF, (width >> 16) & 0xFF, (width >> 24) & 0xFF,
    height & 0xFF, (height >> 8) & 0xFF, (height >> 16) & 0xFF, (height >> 24) & 0xFF,
    color_planes & 0xFF, (color_planes >> 8) & 0xFF,
    bits_per_pixel & 0xFF, (bits_per_pixel >> 8) & 0xFF,
    compression & 0xFF, (compression >> 8) & 0xFF, (compression >> 16) & 0xFF, (compression >> 24) & 0xFF,
    pixel_data_size & 0xFF, (pixel_data_size >> 8) & 0xFF, (pixel_data_size >> 16) & 0xFF, (pixel_data_size >> 24) & 0xFF,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
  };
  if (fwrite(info_header, 1, sizeof(info_header), file) != sizeof(info_header)) {
    fclose(file);
    return false;
  }
  const uint8_t padding_bytes[3] = {0, 0, 0};
  for (int32_t y = height - 1; y >= 0; --y) {
    for (int32_t x = 0; x < width; ++x) {
      Color pixel = grid[y * width + x];
      uint8_t bgr[3] = {
        pixel & 0xFF,
        (pixel >> 8) & 0xFF,
        (pixel >> 16) & 0xFF
      };
      if (fwrite(bgr, 1, bytes_per_pixel, file) != (size_t)bytes_per_pixel) {
        fclose(file);
        return false;
      }
    }
    if (padding_size > 0) {
      if (fwrite(padding_bytes, 1, padding_size, file) != (size_t)padding_size) {
        fclose(file);
        return false;
      }
    }
  }
  fclose(file);
  return true;
}

int main(void) {
  constexpr int32_t width = 640;
  constexpr int32_t height = 480;
  constexpr Color background_color = 0x001A233A;
  constexpr Color line_color_1     = 0x00E63946;
  constexpr Color line_color_2     = 0x00457B9D;
  constexpr Color border_color     = 0x00A8DADC;
  printf("Allocating canvas of size %dx%d...\n", width, height);
  Color* canvas = malloc((size_t)width * (size_t)height * sizeof(Color));
  if (canvas == nullptr) {
    fprintf(stderr, "Fatal error: Failed to allocate frame memory.\n");
    return EXIT_FAILURE;
  }
  for (int32_t i = 0; i < width * height; ++i) {
    canvas[i] = background_color;
  }
  printf("Drawing bounding frame boundaries...\n");
  draw_line(canvas, width, height, 0, 0, width - 1, 0, border_color);
  draw_line(canvas, width, height, 0, height - 1, width - 1, height - 1, border_color);
  draw_line(canvas, width, height, 0, 0, 0, height - 1, border_color);
  draw_line(canvas, width, height, width - 1, 0, width - 1, height - 1, border_color);
  printf("Drawing test lines into frame buffer...\n");
  draw_line(canvas, width, height, 20, 20, width - 21, height - 21, line_color_1);
  draw_line(canvas, width, height, 20, height - 21, width - 21, 20, line_color_2);
  const char* output_path = "output.bmp";
  printf("Exporting image matrix to disk target: '%s'...\n", output_path);
  bool status = save_grid_to_bmp(output_path, canvas, width, height);
  free(canvas);
  canvas = nullptr;
  if (status) {
    printf("Success! Test image compiled and verified successfully.\n");
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Error: Export subsystem failed to construct target file.\n");
    return EXIT_FAILURE;
  }
}
