#pragma once

#include "config.hpp"
#include <iostream>
#include <map>
#include <mutex>
#include <raylib.h>
#include <vector>

struct RectBase {
    Color rect_color = GRAY;
    Color border_color = WHITE;
    float border_size = 1;

    void draw_rect(Vector2 at, Vector2 size);
};

struct TrailBase {
    float trail_speed = KBMVConfig.trail.trail_speed;

    // i should make this a struct but i cba so [0] = position, [1] = height
    float active_trail[2];
    std::vector<float[2]> trails;

    void draw_trails(Vector2 at, float width);
    void begin_trail();
    void finish_trail();
};

enum LayoutItemType { LAYOUT_NONE, LAYOUT_KEYBOARD, LAYOUT_MOUSE, LAYOUT_MAX };

struct LayoutItemBase : RectBase, TrailBase {
    LayoutItemType type;

    float x = 0, y = 0;
    float width = 50, height = 50;

    bool position_set = false;
    float true_x = 0, true_y = 0;

    std::string parent_id = "";
    LayoutItemBase *parent = nullptr;

    virtual void draw() {};
    void update_position();
};

struct LayoutItemNone : LayoutItemBase {
    LayoutItemType type = LAYOUT_NONE;

    void draw() override {};
};

struct LayoutItemKey : LayoutItemBase {
    LayoutItemType type = LAYOUT_KEYBOARD;

    void draw() override;
};

struct LayoutItemMouse : LayoutItemBase {
    LayoutItemType type = LAYOUT_MOUSE;

    void draw() override;
};

typedef struct {
    bool use_mouse;
    bool use_keyboard;

    std::map<std::string, LayoutItemBase *> id_map;
    std::vector<LayoutItemBase *> items;
} layout_t;

extern layout_t KBMVLayout;
extern std::mutex LayoutMutex;

void ClearLayout();