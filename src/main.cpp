#include "main.hpp"

#include "config.hpp"
#include "input.hpp"
#include "window.hpp"
#include <filesystem>
#include <iostream>
#include <thread>

std::atomic_bool IsRunning(true);

int main(int argc, char **argv) {
    LoadDefaultConfig();

    bool config_exists = std::filesystem::exists("kbmv.toml");
    if (!config_exists) {
        WriteDefaultConfig("kbmv.toml");
    } else {
        LoadConfig("kbmv.toml");
    }

    std::thread window_thread(&WindowThread);
    std::thread input_thread(&InputThread);

    window_thread.join();
    IsRunning.store(false);

    input_thread.join();

    std::cout << "Bye bye!" << std::endl;
}