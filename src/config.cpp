#include "config.hpp"
#include "src/layout.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <string_view>
#include <toml++/toml.hpp>

#define PARSE_CONFIG_PART(DST, SRC) DST = SRC.value_or(DST)

using namespace std::string_view_literals;

static constexpr std::string_view default_config = R"(#kbmv.toml
# This is the default KBMV config!
# You may edit this file to your liking.
# If KBMV updates, new values will _not_ automatically be added.
# You can recreate this by moving the existing file elsewhere.

[window]
width = 600
height = 800

[trail] # default trail settings
trail_offset = 1
trail_speed = 500

[rect] # default rect settings
rect_color = "#ff000080"
border_color = "#ff0000"
border_size = 2

[[layout]]
type = "none"
id = "anchor"
x = 8
y = 32

[[layout]]
type = "key"
id = "k1"
parent = "anchor"

[[layout]]
type = "key"
id = "k2"
parent = "k1"
x = 60

[[layout]]
type = "key"
id = "k3"
parent = "k2"
x = 60

)"sv;

config_t KBMVConfig{.window{.width = 800, .height = 600}};

// Function Declarations
void LoadDefaultConfig();
void WriteDefaultConfig(const char *filename);
void LoadConfig(const char *filename);
void ParseConfig(toml::table table);
void ReadColor(std::string hex_str, Color *color);

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
    {
        PARSE_CONFIG_PART(KBMVConfig.window.width, table["window"]["width"]);
        PARSE_CONFIG_PART(KBMVConfig.window.height, table["window"]["height"]);
    }

    {
        PARSE_CONFIG_PART(KBMVConfig.trail.trail_offset,
                          table["trail"]["trail_offset"]);
        PARSE_CONFIG_PART(KBMVConfig.trail.trail_speed,
                          table["trail"]["trail_speed"]);
    }

    {
        std::string rect_color, border_color;
        PARSE_CONFIG_PART(rect_color, table["rect"]["rect_color"]);
        PARSE_CONFIG_PART(border_color, table["rect"]["border_color"]);
        ReadColor(rect_color, &KBMVConfig.rect.rect_color);
        ReadColor(border_color, &KBMVConfig.rect.border_color);

        PARSE_CONFIG_PART(KBMVConfig.rect.border_size,
                          table["rect"]["border_size"]);
    }

    {
        bool has_layout = table["layout"].is_array_of_tables();
        ClearLayout();

        if (!has_layout) {
            return;
        }

        toml::array *layout = table["layout"].as_array();
        layout->for_each([](auto &&element) {
            toml::table item = *element.as_table();
            const char *type = item["type"].value_or("");

            LayoutItemBase *layout_item = nullptr;

            if (strcmp(type, "none") == 0) {
                // it's a dummy exclusively used for parenting
                layout_item = new LayoutItemNone;
            } else if (strcmp(type, "key") == 0) {
                KBMVLayout.use_keyboard = true;
                LayoutItemKey *key_item = new LayoutItemKey;
                layout_item = key_item;

                // TODO: keycode
            } else if (strcmp(type, "mouse") == 0) {
                KBMVLayout.use_mouse = true;
                LayoutItemMouse *mouse_item = new LayoutItemMouse;
                layout_item = mouse_item;

                // TODO: mouse handling
            } else {
                // idk what this is
                std::cerr << "Unknown layout item type '" << type << "'"
                          << std::endl
                          << "Accepted values are: 'key', 'mouse', 'none'"
                          << std::endl;
            }

            PARSE_CONFIG_PART(layout_item->x, item["x"]);
            PARSE_CONFIG_PART(layout_item->y, item["y"]);
            PARSE_CONFIG_PART(layout_item->width, item["width"]);
            PARSE_CONFIG_PART(layout_item->height, item["height"]);

            std::string rect_color, border_color;
            PARSE_CONFIG_PART(rect_color, item["rect_color"]);
            PARSE_CONFIG_PART(border_color, item["border_color"]);
            ReadColor(rect_color, &layout_item->rect_color);
            ReadColor(border_color, &layout_item->border_color);

            PARSE_CONFIG_PART(layout_item->border_size, item["border_size"]);

            std::string parent_id = item["parent"].value_or(std::string{});
            layout_item->parent_id = parent_id;
            if (KBMVLayout.id_map.find(parent_id) != KBMVLayout.id_map.end()) {
                layout_item->parent = KBMVLayout.id_map[parent_id];
            }

            std::string id = item["id"].value_or(std::string{});
            if (KBMVLayout.id_map.find(id) != KBMVLayout.id_map.end()) {
                std::cerr << "Conflicting layout IDs! '" << id << "'"
                          << std::endl;
                return;
            }

            if (!id.empty()) {
                KBMVLayout.id_map[id] = layout_item;
                for (LayoutItemBase *other_item : KBMVLayout.items) {
                    if (id != other_item->parent_id)
                        continue;
                    other_item->parent = layout_item;
                }
            }

            KBMVLayout.items.push_back(layout_item);
        });
    }
}

void ReadColor(std::string hex_str, Color *color) {
    static std::regex pattern(
        "#([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})?");
    std::smatch match;
    if (std::regex_match(hex_str, match, pattern)) {
        unsigned char r, g, b, a = 255;
        sscanf(match[1].str().c_str(), "%2hhx", &r);
        sscanf(match[2].str().c_str(), "%2hhx", &g);
        sscanf(match[3].str().c_str(), "%2hhx", &b);
        if (!match[4].str().empty())
            sscanf(match[4].str().c_str(), "%2hhx", &a);
        *color = Color{r, g, b, a};
    }
}