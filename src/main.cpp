#include "main.hpp"

#include "config.hpp"
#include "input.hpp"
#include "window.hpp"
#include <filesystem>
#include <iostream>
#include <thread>

#ifdef WIN32
#include <Windows.h>
#include <cstdio>
#endif

std::atomic_bool IsRunning(true);

int main(int argc, char **argv) {
    std::cout << "Loading default config..." << std::endl;
    LoadDefaultConfig();

    const char *default_config_name = "kbmv.toml";
#ifdef WIN32
    wchar_t exe_path[FILENAME_MAX];
    GetModuleFileName(nullptr, exe_path, FILENAME_MAX);
    std::filesystem::path exe_path =
        std::filesystem::path(std::string(exe_path));
#else
    std::filesystem::path exe_path =
        std::filesystem::read_symlink("/proc/self/exe");
#endif
    const char *config_filename =
        exe_path.replace_filename(default_config_name).c_str();

    bool write_default_config = true;
    if (argc == 2) {
        config_filename = argv[1];
        write_default_config = false;
    }

    bool config_exists = std::filesystem::exists(config_filename);
    if (!config_exists) {
        if (write_default_config) {
            std::cout << "Writing default config... " << config_filename
                      << std::endl;
            WriteDefaultConfig(config_filename);
        }
    } else {
        std::cout << "Loading user config..." << std::endl;
        LoadConfig(config_filename);
    }

    std::thread window_thread(&WindowThread);
    std::thread input_thread(&InputThread);

    window_thread.join();
    IsRunning.store(false);

    input_thread.join();

    std::cout << "Bye bye!" << std::endl;
}
