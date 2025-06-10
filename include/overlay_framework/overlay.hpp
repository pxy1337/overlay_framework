#pragma once

#include <string>

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <winuser.h>
#include <dwmapi.h>

#include "color.hpp"
#include "math.hpp"
#include "direct2d.hpp"
#include "config.hpp"
#include "window.hpp"

namespace overlay_framework {
    class overlay_t {
    public:
        overlay_t(const overlay_config_t& overlay_config = overlay_config_t());
        ~overlay_t();

        // Remove copy constructor/assign operator
        overlay_t(const overlay_t&) = delete;
        overlay_t& operator=(const overlay_t&) = delete;

        friend class window_t;

    public:
        using callback_t = void (*)(overlay_t*);
        void run(callback_t on_paint_callback = nullptr);

    public:
        // Get public informations about the overlay
        [[nodiscard]] uint32_t get_width() const;
        [[nodiscard]] uint32_t get_height() const;
        [[nodiscard]] math::vec2_t<uint32_t> get_size() const;
        [[nodiscard]] float get_frame_time() const;

        // Public render functions
        void draw_rect_filled(int x, int y, int w, int h, int r, int g, int b, int a);
        void draw_rect_filled(const math::vec2_t<int32_t>& position, const math::vec2_t<int32_t>& size,
                              const color_t& color);

        void draw_text(const std::string& text, int x, int y, int r, int g, int b, int a, bool centered,
                       IDWriteTextFormat* font = nullptr);
        void draw_text(const std::string& text, const math::vec2_t<int32_t>& position, const color_t& color,
                       bool centered, IDWriteTextFormat* font = nullptr);

    private:
        LRESULT window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        overlay_config_t m_overlay_config{};

        direct2d_resources_t m_direct2d{};
        window_t m_window{};

        bool m_is_initialized{};
        bool m_is_running{};
        callback_t m_on_paint_callback{};

        float m_frame_time{};
    };
} // namespace overlay_framework
