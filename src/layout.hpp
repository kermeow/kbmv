#pragma once

#include "config.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <map>
#include <mutex>
#include <raylib.h>
#include <vector>

enum LayoutItemType { LAYOUT_NONE, LAYOUT_KEYBOARD, LAYOUT_MOUSE, LAYOUT_MAX };

struct LayoutItemBase {
    LayoutItemType type;

    float x = 0, y = 0;
    float width = 50, height = 50;

    bool position_set = false;
    float true_x = 0, true_y = 0;

    std::string parent_id = "";
    LayoutItemBase *parent = nullptr;

    bool pressed = false;

    Color rect_color = KBMVConfig.rect.rect_color;
    Color border_color = KBMVConfig.rect.border_color;
    float border_size = KBMVConfig.rect.border_size;

    Color rect_color_pressed = KBMVConfig.rect.rect_color_pressed;
    Color border_color_pressed = KBMVConfig.rect.border_color_pressed;
    // float border_size_pressed = KBMVConfig.rect.border_size;

    float trail_offset = KBMVConfig.trail.trail_offset;
    float trail_speed = KBMVConfig.trail.trail_speed;

    // i should make this a struct but i cba so [0] = position, [1] = height
    float *active_trail = nullptr;
    std::vector<float *> trails;

    virtual void draw() {};
    void update_position();

    void draw_rect();
    void draw_trails();
    void begin_trail();
    void finish_trail();
};

struct LayoutItemNone : LayoutItemBase {
    LayoutItemNone() { this->type = LAYOUT_NONE; }

    void draw() override {};
};

struct LayoutItemKey : LayoutItemBase {
    LayoutItemKey() { this->type = LAYOUT_KEYBOARD; }

    sf::Keyboard::Key key;

    void draw() override;
};

struct LayoutItemMouse : LayoutItemBase {
    LayoutItemMouse() { this->type = LAYOUT_MOUSE; }

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