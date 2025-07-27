#include "layout.hpp"

#include "config.hpp"
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

// LayoutItemBase
void LayoutItemBase::draw_rect() {
    Rectangle rect{.x = this->true_x,
                   .y = this->true_y,
                   .width = this->width,
                   .height = this->height};
    Color rect_color = this->rect_color;
    Color border_color = this->border_color;
    if (this->pressed) {
        rect_color = this->rect_color_pressed;
        border_color = this->border_color_pressed;
    }
    DrawRectangleRec(rect, rect_color);
    if (this->border_size > 0)
        DrawRectangleLinesEx(rect, this->border_size, border_color);
}

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

void LayoutItemBase::draw_trails() {
    if (!this->trail_enabled)
        return;

    if (this->active_trail != nullptr) {
        Rectangle rect{.x = this->true_x,
                       .y = this->true_y + trail_offset * this->height,
                       .width = this->width,
                       .height = this->active_trail[1] * this->trail_speed};
        DrawRectangleRec(rect, this->rect_color_pressed);
    }

    for (float *trail : this->trails) {
        Rectangle rect{.x = this->true_x,
                       .y = this->true_y + (trail_offset * this->height) +
                            trail[0] * this->trail_speed,
                       .width = this->width,
                       .height = trail[1] * this->trail_speed};
        DrawRectangleRec(rect, this->rect_color_pressed);
    }

    float max_y = KBMVConfig.window.height;
    while (this->trails.size() > 0) {
        float *trail = trails[0];
        float y = this->true_y + (trail_offset * this->height) +
                  trail[0] * this->trail_speed;
        if (y < max_y)
            break;
        this->trails.erase(this->trails.begin());
        delete trail;
    }
}

void LayoutItemBase::begin_trail() {
    if (!this->trail_enabled)
        return;
    this->active_trail = new float[2];
    this->active_trail[0] = 0;
    this->active_trail[1] = 0;
}

void LayoutItemBase::finish_trail() {
    if (!this->trail_enabled)
        return;
    this->trails.push_back(this->active_trail);
    this->active_trail = nullptr;
}

// LayoutItemKey
void LayoutItemKey::draw() {
    this->draw_trails();
    this->draw_rect();
}

// LayoutItemMouse
void LayoutItemMouse::draw() {
    this->draw_trails();
    this->draw_rect();
}
