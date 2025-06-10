#pragma once

#include <numbers>

#include "vec2_t.hpp"
#include "vec3_t.hpp"

namespace overlay_framework {
    namespace math {
        template <std::floating_point T = float>
        [[nodiscard]] constexpr inline T deg_to_rad(T degrees) noexcept {
            return degrees * std::numbers::pi_v<T> / static_cast<T>(180.0);
        }

        template <std::floating_point T = float>
        [[nodiscard]] constexpr inline T rad_to_deg(T radians) noexcept {
            return radians * static_cast<T>(180.0) / std::numbers::pi_v<T>;
        }
    } // namespace math
} // namespace overlay_framework
