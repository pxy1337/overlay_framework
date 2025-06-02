#pragma once

#include <string>
#include <expected>

namespace overlay_framework {
    struct overlay_config_t {
        std::string window_name{ "OverlayWindow" };
        std::string window_class_name{ "OverlayWindowClass" };
        bool vsync{ false };
    };

    class overlay_t {
    public:
        overlay_t(const overlay_config_t& overlay_config = overlay_config_t());

    public:
        
    private:

    };
} // namespace overlay_framework
