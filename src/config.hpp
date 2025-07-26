#pragma once

#include <raylib.h>

typedef struct {
    int width;
    int height;
} window_config_t;

typedef struct {
    float trail_offset;
    float trail_speed;
} trail_config_t;

typedef struct {
    Color rect_color;
    Color border_color;
    Color rect_color_pressed;
    Color border_color_pressed;
    float border_size;
} rect_config_t;

typedef struct {
    window_config_t window;
    trail_config_t trail;
    rect_config_t rect;
} config_t;

extern config_t KBMVConfig;

void LoadDefaultConfig();
void WriteDefaultConfig(const char *filename);
void LoadConfig(const char *filename);