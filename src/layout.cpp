#include "layout.hpp"

#include <iostream>
#include <mutex>
#include <raylib.h>
#include <vector>

layout_t KBMVLayout;
std::mutex LayoutMutex;

// Function Declarations
void ClearLayout();

// Function Definitions
void ClearLayout() {
    LayoutMutex.lock();

    KBMVLayout.use_mouse = false;
    KBMVLayout.use_keyboard = false;

    KBMVLayout.id_map.clear();
    delete[] KBMVLayout.items.data();
    KBMVLayout.items.clear();

    LayoutMutex.unlock();
}

// RectBase
void RectBase::draw_rect(Vector2 at, Vector2 size) {
    Rectangle rect{.x = at.x, .y = at.y, .width = size.x, .height = size.y};
    DrawRectangleRec(rect, this->rect_color);
    if (this->border_size > 0)
        DrawRectangleLinesEx(rect, this->border_size, this->border_color);
}

// TrailBase
void TrailBase::draw_trails(Vector2 at, float width) {}
void TrailBase::begin_trail() {
    this->active_trail = new float[2];
}
void TrailBase::finish_trail() {
    this->trails.push_back(this->active_trail);
    this->active_trail = nullptr;
}

// LayoutItemBase
void LayoutItemBase::update_position() {
    if (this->position_set)
        return;
    this->position_set = true;

    this->true_x = this->x;
    this->true_y = this->y;

    if (this->parent != nullptr) {
        this->parent->update_position();
        this->true_x += this->parent->true_x;
        this->true_y += this->parent->true_y;
    }
}

// LayoutItemKey
void LayoutItemKey::draw() {
    Vector2 position{.x = this->true_x, .y = this->true_y};
    Vector2 size{.x = this->width, .y = this->height};

    this->draw_trails(position, this->width);
    this->draw_rect(position, size);
}

// LayoutItemMouse
void LayoutItemMouse::draw() { }