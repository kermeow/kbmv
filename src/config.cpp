#include "config.hpp"

#include <fstream>
#include <iostream>
#include <string_view>
#include <toml++/impl/parse_error.hpp>
#include <toml++/impl/parser.hpp>
#include <toml++/toml.hpp>

#define PARSE_CONFIG_PART(DST, SRC) DST = SRC.value_or(DST)

using namespace std::string_view_literals;

static constexpr std::string_view default_config = R"(#kbmv.toml
# This is the default KBMV config!
# You may edit this file to your liking.
# If KBMV updates, new values will _not_ automatically be added.
# You can recreate this by moving the existing file elsewhere.

[window]
width = 800
height = 600
)"sv;

config_t KBMVConfig{.window{.width = 800, .height = 600}};

// Function Declarations
void LoadDefaultConfig();
void WriteDefaultConfig(const char *filename);
void LoadConfig(const char *filename);
void ParseConfig(toml::table table);

// Function Definitions
void LoadDefaultConfig() {
    toml::table table = toml::parse(default_config);
    ParseConfig(table);
}

void WriteDefaultConfig(const char *filename) {
    std::ofstream output_file(filename);

    if (!output_file.is_open()) {
        std::cerr << "Failed to write default config!" << std::endl;
        return;
    }

    output_file << default_config;
    output_file.close();
}

void LoadConfig(const char *filename) {
    toml::table table;
    try {
        table = toml::parse_file(filename);
        ParseConfig(table);
    } catch (const toml::parse_error &error) {
        LoadDefaultConfig();
        std::cerr << "Failed to load config!" << std::endl
                  << error << std::endl;
    }
}

void ParseConfig(toml::table table) {
    PARSE_CONFIG_PART(KBMVConfig.window.width, table["window"]["width"]);
    PARSE_CONFIG_PART(KBMVConfig.window.height, table["window"]["height"]);
}