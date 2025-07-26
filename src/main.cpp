#include "raylib.h"
#include <iostream>
#include <thread>

// Function Declarations
int main(int argc, char **argv);

void WindowThread();
void InputThread();

// Function Definitions
int main(int argc, char **argv) {
    std::thread window_thread(&WindowThread);
    std::thread input_thread(&InputThread);

    window_thread.join();
    input_thread.join();

    std::cout << "Bye bye!" << std::endl;
}

void WindowThread() {
    int width = 800, height = 600;
    InitWindow(width, height, "kbmv");

    SetWindowMinSize(width, height);
    SetWindowMaxSize(width, height);

    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{.a = 0});
        DrawFPS(8, 8);
        EndDrawing();
    }
}

void InputThread() { std::cout << "Look ma! I'm threaded too!" << std::endl; }