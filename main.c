#include "raylib/include/raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MARKER_SIZE 25.0f
#define SCREEN_FPS 60
#define DELAY_SEC (1.0f / SCREEN_FPS)
#define DELAY_MS ((Uint32)floorf(DELAY_SEC * 1000.0f))
// vact2 constructor
Vector2 vec2_constr(float a, float b) { return (Vector2){a, b}; }

// vect2 operataions
Vector2 vec2_sub(Vector2 a, Vector2 b) {
  return vec2_constr(a.x - b.x, a.y - b.y);
}

Vector2 vec2_add(Vector2 a, Vector2 b) {
  return vec2_constr(a.x + b.x, a.y + b.y);
}

Vector2 vec2_scale(Vector2 a, float s) { return vec2_constr(a.x * s, a.y * s); }

// lerp function 1d
float lerpf(float a, float b, float p) { return a + (b - a) * p; }

// lerp fuction 2d
Vector2 lerpv2(Vector2 a, Vector2 b, float p) {
  return vec2_add(a, vec2_scale(vec2_sub(b, a), p));
}

// marker fucntion
void render_marker(Vector2 position, Color color) {
  Vector2 size = vec2_constr(MARKER_SIZE, MARKER_SIZE);

  DrawRectangleV(vec2_sub(position, vec2_scale(size, 0.5)), size, color);
}
// bezier

Vector2 beziern_sample(Vector2 *ps, Vector2 *xs, size_t n, float p) {
  memcpy(xs, ps, n * sizeof(Vector2));
  // xs[i] = lerpv2(xs[i], xs[i+1], p);
  while (n > 1) {
    for (size_t i = 0; i < n - 1; i++) {
      xs[i] = lerpv2(xs[i], xs[i + 1], p);
    }
    n--;
  }
  return xs[0];
}

void render_bezier_markers(Vector2 *ps, Vector2 *xs, size_t n, float s) {
  for (float p = 0.0f; p <= 1.0f; p += s) {
    render_marker(beziern_sample(ps, xs, n, p), PURPLE);
  }
}

void render_bezier_curve(Vector2 *ps, Vector2 *xs, size_t n, float s) {
  for (float p = 0.0f; p + s <= 1.0f; p += s) {
    Vector2 begin = beziern_sample(ps, xs, n, p);
    Vector2 end = beziern_sample(ps, xs, n, p + s);
    DrawLineV(begin, end, PURPLE);
  }
}
// ps

#define PS_CAPACITY 256
Vector2 ps[PS_CAPACITY];
Vector2 xs[PS_CAPACITY];
size_t ps_count = 0;
int ps_selected = -1;

// ps at
int ps_at(Vector2 pos) {
  const Vector2 ps_size = vec2_constr(MARKER_SIZE, MARKER_SIZE);
  for (size_t i = 0; i < ps_count; i++) {
    const Vector2 ps_begin = vec2_sub(ps[i], vec2_scale(ps_size, 0.5f));
    const Vector2 ps_end = vec2_add(ps_begin, ps_size);
    if (ps_begin.x <= pos.x && pos.x <= ps_end.x && ps_begin.y <= pos.y &&
        pos.y <= ps_end.y) {
      return (int)i;
    }
  }
  return -1;
}

// main funciton
int main() {
  const int screenWidth = 800;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "Quadratic Bezier Curve");

  Vector2 begin = {100.0f, 100.0f};
  Vector2 end = {250.0f, 400.0f};
  int j = 0;
  SetTargetFPS(SCREEN_FPS);
  float time = 0.0f;
  int markers = 1;
  float bezier_sample_step = 0.01f;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    if (IsKeyPressed(KEY_F1)) {
      markers = !markers;
    }
    // TODO: with the mouse wheel adjust the bezier_sample_step
    // detect the mouse wheel event
    int wheel = GetMouseWheelMove();
    if (wheel != 0) {
      bezier_sample_step += 0.01f * wheel;
      if (bezier_sample_step < 0.01f) {
        bezier_sample_step = 0.01f;
      }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Vector2 mouse_pos = GetMousePosition();
      ps_selected = ps_at(mouse_pos);
      if (ps_selected < 0) {
        ps[ps_count++] = vec2_constr(mouse_pos.x, mouse_pos.y);
      } else {
        printf("ps_selected : %d\n", ps_selected);
      }
    } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      Vector2 mouse_pos = GetMousePosition();
      if (ps_selected != -1) {
        ps[ps_selected] = mouse_pos;
      }
    }
    const float p = (sinf(time) + 1.0f) * 0.5f;

    // TODO: : switch between markers and curve at runtime
    if (ps_count >= 1) {
      if (markers) {
        render_bezier_markers(ps, xs, ps_count, bezier_sample_step);
      } else {
        render_bezier_curve(ps, xs, ps_count, bezier_sample_step);
      }
    }
    for (size_t i = 0; i < ps_count; i++) {
      render_marker(ps[i], RED);
      if (i < ps_count - 1) {
        DrawLineV(ps[i], ps[i + 1], RED);
      }
    }
    EndDrawing();
    time += DELAY_SEC;
  }

  CloseWindow();

  return 0;
}
