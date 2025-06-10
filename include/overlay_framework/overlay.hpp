#pragma once

#include <string>
#include <expected>

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <winuser.h>
#include <dwmapi.h>

#include "color.hpp"
#include "math.hpp"

namespace overlay_framework {
    struct overlay_config_t {
        std::wstring window_name{ L"OverlayWindow" };
        std::wstring window_class_name{ L"OverlayWindowClass" };
        bool vsync{ false };
    };

    class overlay_t;

    using callback_t = void (*)(overlay_t*);

    class overlay_t {
    public:
        overlay_t(const overlay_config_t& overlay_config = overlay_config_t());
        ~overlay_t();

        // Remove copy constructor/assign operator
        overlay_t(const overlay_t&) = delete;
        overlay_t& operator=(const overlay_t&) = delete;

    public:
        void run(callback_t on_paint_callback = nullptr);

    public:
        // Desc: Get 'desired' font
        // Note: there is no way to request custom fonts
        // TODO: add and option to request custom fonts
        // and a better api for the entire font system
        IDWriteTextFormat* get_verdana_regular();
        IDWriteTextFormat* get_verdana_bold();

        // Get public informations about the overlay
        uint32_t get_width() const;
        uint32_t get_height() const;
        std::pair<uint32_t, uint32_t> get_size() const;
        float get_frame_time() const;

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
        // Desc: Direct2D/DWrite functions
        // TODO: Move to separate class
        std::expected<void, std::string> initialize_direct2d(HWND hwnd);
        void cleanup_direct2d();

    private:
        overlay_config_t m_overlay_config{};
        HINSTANCE m_instance_handle{};
        HWND m_window_handle{};

        ID2D1Factory* m_d2d_factory{};
        IDWriteFactory* m_dwrite_factory{};
        ID2D1HwndRenderTarget* m_render_target{};

        IDWriteTextFormat* m_verdana_regular{};
        IDWriteTextFormat* m_verdana_bold{};

        ID2D1SolidColorBrush* m_brush{};

        bool m_is_initialized{};
        bool m_is_running{};
        callback_t m_on_paint_callback{};

        uint32_t m_width{};
        uint32_t m_height{};

        float m_frame_time{};
    };
} // namespace overlay_framework
