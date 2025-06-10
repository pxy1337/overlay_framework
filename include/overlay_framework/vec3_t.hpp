#pragma once

#include <concepts>
#include <cmath>

namespace overlay_framework {
    namespace math {
        template <typename T = float>
            requires std::integral<T> || std::floating_point<T>
        struct vec3_t {
        public:
            T x{};
            T y{};
            T z{};

        public:
            vec3_t() = default;
            vec3_t(T x, T y, T z) : x(x), y(y), z(z) {}
            vec3_t(T xyz) : x(xyz), y(xyz), z(xyz) {}

        public:
            vec3_t operator+(const vec3_t& o) const {
                return { x + o.x, y + o.y, z + o.z };
            }

            vec3_t operator-(const vec3_t& o) const {
                return { x - o.x, y - o.y, z - o.z };
            }

            vec3_t operator*(const vec3_t& o) const {
                return { x * o.x, y * o.y, z * o.z };
            }

            vec3_t operator*(float f) const {
                return { x * f, y * f, z * f };
            }

            vec3_t operator/(const vec3_t& o) const {
                return { x / o.x, y / o.y, z / o.z };
            }

            vec3_t operator/(float f) const {
                return { x / f, y / f, z / f };
            }

            vec3_t& operator+=(const vec3_t& o) {
                x += o.x;
                y += o.y;
                z += o.z;
                return *this;
            }

            vec3_t& operator-=(const vec3_t& o) {
                x -= o.x;
                y -= o.y;
                z -= o.z;
                return *this;
            }

            vec3_t& operator*=(const vec3_t& o) {
                x *= o.x;
                y *= o.y;
                z *= o.z;
                return *this;
            }

            vec3_t& operator/=(const vec3_t& o) {
                x /= o.x;
                y /= o.y;
                z /= o.z;
                return *this;
            }

        public:
            bool is_zero() const {
                return x == static_cast<T>(0) && y == static_cast<T>(0) && z == static_cast<T>(0);
            }

            bool is_valid() const {
                return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
            }

            T magnitude() const {
                return std::sqrt(x * x + y * y + z * z);
            }

            T magnitude_squared() const {
                return x * x + y * y + z * z;
            }

            T dot_product(const vec3_t& o) const {
                return x * o.x + y * o.y + z * o.z;
            }

            vec3_t cross_product(const vec3_t& o) {
                return { y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x };
            }

            T distance_to(const vec3_t& o) const {
                return (o - *this).magnitude();
            }

            vec3_t normalized() const {
                const auto m = magnitude();
                return { x / m, y / m, z / m };
            }

            vec3_t& normalize() {
                const auto m = magnitude();
                x /= m;
                y /= m;
                z /= m;
                return *this;
            }
        };
    } // namespace math
} // namespace overlay_framework
