#pragma once

#include <cstdint>
#include <string>
#include <windows.h>
#include <dwmapi.h>
#include <expected>

#include "overlay_framework/math.hpp"

namespace overlay_framework {
    class overlay_t;

    class window_t {
    public:
        window_t() = default;
        ~window_t();

        window_t(const window_t&) = delete;
        window_t& operator=(const window_t&) = delete;

        window_t(window_t&&) noexcept = default;
        window_t& operator=(window_t&&) noexcept = default;

        [[nodiscard]] std::expected<void, std::string> initialize(overlay_t* ov, const std::wstring& window_name,
                                                                  const std::wstring& class_name);

        [[nodiscard]] uint32_t get_width() const {
            return m_size.x;
        }

        [[nodiscard]] uint32_t get_height() const {
            return m_size.y;
        }

        [[nodiscard]] math::vec2_t<uint32_t> get_size() const {
            return m_size;
        }

        [[nodiscard]] HWND get_handle() const {
            return m_window_handle;
        }

        void set_size(const math::vec2_t<uint32_t> new_size) {
            m_size = new_size;
        }

        bool process_messages();
        static LRESULT CALLBACK static_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        std::wstring m_window_name{};
        std::wstring m_class_name{};
        HINSTANCE m_instance_handle{};
        HWND m_window_handle{};

        math::vec2_t<uint32_t> m_size{};
    };
} // namespace overlay_framework
