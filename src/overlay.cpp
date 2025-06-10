#include "overlay_framework/overlay.hpp"

#include <chrono>

#include <fmt/core.h>

namespace overlay_framework {
    overlay_t::overlay_t(const overlay_config_t& config) : m_overlay_config(config) {
        m_instance_handle = GetModuleHandle(nullptr);

        WNDCLASSEX wc{};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = s_wnd_proc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_instance_handle;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(RGB(0, 0, 0));
        wc.lpszMenuName = NULL;
        wc.lpszClassName = m_overlay_config.window_class_name.c_str();
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        RegisterClassEx(&wc);

        const auto ex_style = WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW;

        m_width = GetSystemMetrics(SM_CXSCREEN);
        m_height = GetSystemMetrics(SM_CYSCREEN);

        m_window_handle =
            CreateWindowEx(ex_style, m_overlay_config.window_class_name.c_str(), m_overlay_config.window_name.c_str(),
                           WS_POPUP, 0, 0, m_width, m_height, NULL, NULL, wc.hInstance, this);

        MARGINS margins = { -1, -1, -1, -1 }; // ?, not sure
        DwmExtendFrameIntoClientArea(m_window_handle, &margins);

        ShowWindow(m_window_handle, SW_SHOW);
        UpdateWindow(m_window_handle);

        SetLayeredWindowAttributes(m_window_handle, 0x000000, 255, LWA_ALPHA);

        m_is_initialized = true;
    }

    overlay_t::~overlay_t() {
        DestroyWindow(m_window_handle);
        UnregisterClass(m_overlay_config.window_class_name.c_str(), m_instance_handle);
    }

    void overlay_t::run(callback_t on_paint_callback) {
        m_is_running = true;
        m_on_paint_callback = on_paint_callback;

        while (m_is_running && process_messages()) {
        }
    }

    bool overlay_t::process_messages() {
        MSG msg{};
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                return false;
        }
        return true;
    }

    LRESULT CALLBACK overlay_t::s_wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_CREATE) {
            LPCREATESTRUCT create_param = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_param->lpCreateParams));
        }

        if (overlay_t* ov = reinterpret_cast<overlay_t*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))) {
            return ov->wnd_proc(hwnd, uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    LRESULT overlay_t::wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_SIZE:
            if (m_direct2d.get_render_target()) {
                RECT rc;
                GetClientRect(hWnd, &rc);

                D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
                m_direct2d.get_render_target()->Resize(size);

                m_width = size.width;
                m_height = size.height;

                InvalidateRect(hWnd, NULL, FALSE);
            }
            return 0;
        case WM_DESTROY:
            m_direct2d.cleanup();
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            if (!m_direct2d.get_render_target()) {
                const auto direct2d_initialized = m_direct2d.initialize(hWnd, m_overlay_config.vsync);
                if (!direct2d_initialized) {
                    MessageBoxA(nullptr, direct2d_initialized.error().c_str(), "[Error]", MB_OK);
                }
            }

            const auto start_timestamp = std::chrono::high_resolution_clock::now();

            m_direct2d.get_render_target()->BeginDraw();
            m_direct2d.get_render_target()->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.f));

            if (m_on_paint_callback) {
                m_on_paint_callback(this);
            }

            const auto hr = m_direct2d.get_render_target()->EndDraw();
            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
                // TODO: Recreate target
                MessageBoxA(nullptr, "render_target->EndDraw() == D2DERR_RECREATE_TARGET", "[Error]", MB_OK);
            }

            const auto end_timestamp = std::chrono::high_resolution_clock::now();

            const auto duration = std::chrono::duration<float, std::milli>(end_timestamp - start_timestamp);

            m_frame_time = duration.count();

            return 0;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void overlay_t::draw_rect_filled(int x, int y, int w, int h, int r, int g, int b, int a) {
        const auto old_color = m_direct2d.get_brush()->GetColor();

        m_direct2d.get_brush()->SetColor(D2D1_COLOR_F{ r / 255.f, g / 255.f, b / 255.f, a / 255.f });

        m_direct2d.get_render_target()->FillRectangle(
            D2D1_RECT_F{ (float)x, (float)y, (float)x + (float)w, (float)y + (float)h }, m_direct2d.get_brush());

        m_direct2d.get_brush()->SetColor(old_color);
    }

    void overlay_t::draw_rect_filled(const math::vec2_t<int32_t>& position, const math::vec2_t<int32_t>& size,
                                     const color_t& color) {
        const auto old_color = m_direct2d.get_brush()->GetColor();

        m_direct2d.get_brush()->SetColor(color.to_direct2d());

        m_direct2d.get_render_target()->FillRectangle(
            D2D1_RECT_F{ static_cast<float>(position.x), static_cast<float>(position.y),
                         static_cast<float>(position.x) + static_cast<float>(size.x),
                         static_cast<float>(position.y) + static_cast<float>(size.y) },
            m_direct2d.get_brush());

        m_direct2d.get_brush()->SetColor(old_color);
    }

    uint32_t overlay_t::get_width() const {
        return m_width;
    }

    uint32_t overlay_t::get_height() const {
        return m_height;
    }

    math::vec2_t<uint32_t> overlay_t::get_size() const {
        return { m_width, m_height };
    }

    float overlay_t::get_frame_time() const {
        return m_frame_time;
    }

    void overlay_t::draw_text(const std::string& text, int x, int y, int r, int g, int b, int a, bool centered,
                              IDWriteTextFormat* font) {
        const auto brush = m_direct2d.get_brush();

        const auto old_color = brush->GetColor();

        brush->SetColor(D2D1_COLOR_F{ (float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)a / 255.f });

        std::wstring wstr(text.begin(), text.end());

        if (!font) {
            font = m_direct2d.get_verdana_bold();
        }

        D2D1_RECT_F rect;

        if (centered) {
            IDWriteTextLayout* text_layout = nullptr;
            HRESULT hr = m_direct2d.get_dwrite_factory()->CreateTextLayout(wstr.c_str(), wstr.size(), font, m_width,
                                                                           m_height, &text_layout);

            if (SUCCEEDED(hr)) {
                DWRITE_TEXT_METRICS metrics;
                text_layout->GetMetrics(&metrics);

                rect = { x - metrics.width / 2.0f, y - metrics.height / 2.0f, x + metrics.width / 2.0f,
                         y + metrics.height / 2.0f };

                text_layout->Release();
            }
        } else {
            rect = { float(x), float(y), 2560, 1440 };
        }

        m_direct2d.get_render_target()->DrawTextW(wstr.c_str(), wstr.size(), font, rect, brush);

        brush->SetColor(old_color);
    }

    void overlay_t::draw_text(const std::string& text, const math::vec2_t<int32_t>& position, const color_t& color,
                              bool centered, IDWriteTextFormat* font) {
        const auto brush = m_direct2d.get_brush();

        const auto old_color = brush->GetColor();

        brush->SetColor(color.to_direct2d());

        std::wstring wstr(text.begin(), text.end());

        if (!font) {
            font = m_direct2d.get_verdana_bold();
        }

        D2D1_RECT_F rect;

        if (centered) {
            IDWriteTextLayout* text_layout = nullptr;
            HRESULT hr = m_direct2d.get_dwrite_factory()->CreateTextLayout(wstr.c_str(), wstr.size(), font, m_width,
                                                                           m_height, &text_layout);

            if (SUCCEEDED(hr)) {
                DWRITE_TEXT_METRICS metrics;
                text_layout->GetMetrics(&metrics);

                rect = { position.x - metrics.width / 2.0f, position.y - metrics.height / 2.0f,
                         position.x + metrics.width / 2.0f, position.y + metrics.height / 2.0f };

                text_layout->Release();
            }
        } else {
            rect = { float(position.x), float(position.y), 2560, 1440 };
        }

        m_direct2d.get_render_target()->DrawTextW(wstr.c_str(), wstr.size(), font, rect, brush);

        brush->SetColor(old_color);
    }
} // namespace overlay_framework
