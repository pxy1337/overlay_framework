#pragma once

#include <concepts>
#include <cmath>

namespace overlay_framework {
    namespace math {
        template <typename T = float>
            requires std::integral<T> || std::floating_point<T>
        struct vec2_t {
        public:
            T x{};
            T y{};

        public:
            vec2_t() = default;
            vec2_t(T x, T y) : x(x), y(y) {}
            vec2_t(T xy) : x(xy), y(xy) {}

        public:
            vec2_t operator+(const vec2_t& o) const {
                return { x + o.x, y + o.y };
            }

            vec2_t operator-(const vec2_t& o) const {
                return { x - o.x, y - o.y };
            }

            vec2_t operator*(const vec2_t& o) const {
                return { x * o.x, y * o.y };
            }

            vec2_t operator*(float f) const {
                return { x * f, y * f };
            }

            vec2_t operator/(const vec2_t& o) const {
                return { x / o.x, y / o.y };
            }

            vec2_t operator/(float f) const {
                return { x / f, y / f };
            }

            vec2_t& operator+=(const vec2_t& o) {
                x += o.x;
                y += o.y;
                return *this;
            }

            vec2_t& operator-=(const vec2_t& o) {
                x -= o.x;
                y -= o.y;
                return *this;
            }

            vec2_t& operator*=(const vec2_t& o) {
                x *= o.x;
                y *= o.y;
                return *this;
            }

            vec2_t& operator/=(const vec2_t& o) {
                x /= o.x;
                y /= o.y;
                return *this;
            }

        public:
            bool is_zero() const {
                return x == static_cast<T>(0) && y == static_cast<T>(0);
            }

            bool is_valid() const {
                return std::isfinite(x) && std::isfinite(y);
            }

            T magnitude() const {
                return std::sqrt(x * x + y * y);
            }

            T magnitude_squared() const {
                return x * x + y * y;
            }

            T distance_to(const vec2_t& o) const {
                return (o - *this).magnitude();
            }
        };
    } // namespace math
} // namespace overlay_framework
