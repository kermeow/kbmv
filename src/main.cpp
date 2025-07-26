#include "main.hpp"

#include "input.hpp"
#include "window.hpp"
#include <iostream>
#include <thread>

std::atomic_bool IsRunning(true);

int main(int argc, char **argv) {
    std::thread window_thread(&WindowThread);
    std::thread input_thread(&InputThread);

    window_thread.join();
    IsRunning.store(false);

    input_thread.join();

    std::cout << "Bye bye!" << std::endl;
}