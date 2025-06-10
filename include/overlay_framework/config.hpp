#pragma once

#include <string>

namespace overlay_framework {
    struct overlay_config_t {
        std::wstring window_name{ L"OverlayWindow" };
        std::wstring window_class_name{ L"OverlayWindowClass" };
        bool vsync{ false };
    };
} // namespace overlay_framework
