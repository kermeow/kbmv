#include "input.hpp"

#include "main.hpp"
#include "src/layout.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <chrono>
#include <iostream>
#include <ratio>
#include <thread>

// Function Declarations
void InputThread();

// Function Definitions
void InputThread() {
    sf::Vector2i mouse_delta;
    sf::Vector2i last_mouse_position = sf::Mouse::getPosition();

    std::chrono::time_point last_frame =
        std::chrono::high_resolution_clock::now();

    while (IsRunning.load()) {
        std::chrono::time_point this_frame =
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> delta_duration =
            this_frame - last_frame;
        double delta_time = delta_duration.count() / 1000;
        last_frame = this_frame;

        // std::cout << delta_time << std::endl;

        if (KBMVLayout.use_mouse) {
            sf::Vector2i mouse_position = sf::Mouse::getPosition();
            mouse_delta = mouse_position - last_mouse_position;
            last_mouse_position = mouse_position;
        }

        LayoutMutex.lock();

        for (LayoutItemBase *item : KBMVLayout.items) {
            switch (item->type) {
            case LAYOUT_KEYBOARD: {
                LayoutItemKey *key_item =
                    reinterpret_cast<LayoutItemKey *>(item);

                for (float *trail : key_item->trails) {
                    trail[0] += delta_time;
                }

                bool pressed = sf::Keyboard::isKeyPressed(key_item->key);

                if (key_item->pressed) {
                    key_item->active_trail[1] += delta_time;
                }

                if (pressed && !key_item->pressed) {
                    // we just pressed the key
                    key_item->begin_trail();
                }

                if (key_item->pressed && !pressed) {
                    // we just released the key
                    key_item->finish_trail();
                }

                key_item->pressed = pressed;
                break;
            }
            case LAYOUT_NONE:
            default:
                break;
            }
        }

        LayoutMutex.unlock();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(250us); // prevent cpu cooking
    }
}