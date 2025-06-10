#pragma once

#include <cstdint>

#include <d2d1.h>

namespace overlay_framework {
    class color_t {
    public:
        color_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

    public:
        static color_t from_hex(uint32_t hex) {
            uint8_t red = (hex >> 24) & 0xFF;
            uint8_t green = (hex >> 16) & 0xFF;
            uint8_t blue = (hex >> 8) & 0xFF;
            uint8_t alpha = hex & 0xFF;

            return { red, green, blue, alpha };
        }

        uint32_t to_hex() const {
            return (static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 16) |
                   (static_cast<uint32_t>(b) << 8) | static_cast<uint32_t>(a);
        }

        D2D1_COLOR_F to_direct2d() const {
            return { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
        }

    public:
        uint8_t r{};
        uint8_t g{};
        uint8_t b{};
        uint8_t a{};
    };

    namespace predefined_colors {
        const inline static color_t red(255, 0, 0);
        const inline static color_t green(0, 255, 0);
        const inline static color_t blue(0, 0, 255);
        const inline static color_t black(0, 0, 0);
        const inline static color_t white(255, 255, 255);
    } // namespace predefined_colors
} // namespace overlay_framework
