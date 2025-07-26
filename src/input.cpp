#include "input.hpp"

#include "main.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <thread>

// Function Declarations
void InputThread();

// Function Definitions
void InputThread() {
    sf::Vector2i mouse_delta;
    sf::Vector2i last_mouse_position = sf::Mouse::getPosition();

    while (IsRunning.load()) {
        sf::Vector2i mouse_position = sf::Mouse::getPosition();
        mouse_delta = mouse_position - last_mouse_position;
        last_mouse_position = mouse_position;

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100us); // prevent cpu cooking
    }
}