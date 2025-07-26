#pragma once

typedef struct {
    int width;
    int height;
} window_config_t;

typedef struct {
    window_config_t window;
} config_t;

extern config_t KBMVConfig;

void LoadDefaultConfig();
void WriteDefaultConfig(const char *filename);
void LoadConfig(const char *filename);