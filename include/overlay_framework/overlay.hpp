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

namespace overlay_framework {
    class overlay_t {
    public:
        overlay_t(const overlay_config_t& overlay_config = overlay_config_t());
        ~overlay_t();

        // Remove copy constructor/assign operator
        overlay_t(const overlay_t&) = delete;
        overlay_t& operator=(const overlay_t&) = delete;

    public:
        using callback_t = void (*)(overlay_t*);
        void run(callback_t on_paint_callback = nullptr);

    public:
        // Get public informations about the overlay
        [[nodiscard]] uint32_t get_width() const;
        [[nodiscard]] uint32_t get_height() const;
        [[nodiscard]] std::pair<uint32_t, uint32_t> get_size() const;
        [[nodiscard]] float get_frame_time() const;

        // Public render functions
        void draw_rect_filled(int x, int y, int w, int h, int r, int g, int b, int a);
        void draw_rect_filled(const math::vec2_t<int32_t>& position, const math::vec2_t<int32_t>& size,
                              const color_t& color);
        void draw_text(const std::string& text, int x, int y, int r, int g, int b, int a, bool centered,
                       IDWriteTextFormat* font = nullptr);

    private:
        // Desc: Window related functions
        // TODO: Move to separate class
        bool process_messages();
        static LRESULT CALLBACK s_wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        overlay_config_t m_overlay_config{};
        HINSTANCE m_instance_handle{};
        HWND m_window_handle{};

        direct2d_resources_t m_direct2d{};

        bool m_is_initialized{};
        bool m_is_running{};
        callback_t m_on_paint_callback{};

        uint32_t m_width{};
        uint32_t m_height{};

        float m_frame_time{};
    };
} // namespace overlay_framework
