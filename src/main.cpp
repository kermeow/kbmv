#include "main.hpp"

#include "config.hpp"
#include "input.hpp"
#include "window.hpp"
#include <filesystem>
#include <iostream>
#include <thread>

std::atomic_bool IsRunning(true);

int main(int argc, char **argv) {
    std::cout << "Loading default config..." << std::endl;
    LoadDefaultConfig();

    bool config_exists = std::filesystem::exists("kbmv.toml");
    if (!config_exists) {
        std::cout << "Writing default config..." << std::endl;
        WriteDefaultConfig("kbmv.toml");
    } else {
        std::cout << "Loading user config..." << std::endl;
        LoadConfig("kbmv.toml");
    }

    std::thread window_thread(&WindowThread);
    std::thread input_thread(&InputThread);

    window_thread.join();
    IsRunning.store(false);

    input_thread.join();

    std::cout << "Bye bye!" << std::endl;
}