#include "input.hpp"
#include "window.hpp"
#include <iostream>
#include <thread>

int main(int argc, char **argv) {
    std::thread window_thread(&WindowThread);
    std::thread input_thread(&InputThread);

    window_thread.join();
    input_thread.join();

    std::cout << "Bye bye!" << std::endl;
}