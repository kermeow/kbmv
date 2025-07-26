#pragma once

#include <raylib.h>

struct window_config_s {
    int width = 800;
    int height = 600;
};

struct trail_config_s {
    bool trail_enabled = true;
    float trail_offset = 1;
    float trail_speed = 400;
};

struct rect_config_s {
    Color rect_color = GRAY;
    Color border_color = WHITE;
    Color rect_color_pressed = WHITE;
    Color border_color_pressed = WHITE;
    float border_size = 1;
};

struct config_s {
    window_config_s window{};
    trail_config_s trail{};
    rect_config_s rect{};
};

extern config_s KBMVConfig;

void LoadDefaultConfig();
void WriteDefaultConfig(const char *filename);
void LoadConfig(const char *filename);