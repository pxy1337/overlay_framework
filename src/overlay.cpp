#include "overlay_framework/overlay.hpp"

#include <chrono>

#include <fmt/core.h>

namespace overlay_framework {
    overlay_t::overlay_t(const overlay_config_t& config) : m_overlay_config(config) {
        const auto window_initialized = m_window.initialize(this, config.window_name, config.window_class_name);
        if (!window_initialized) {
            MessageBoxA(nullptr, window_initialized.error().c_str(), "[Error]", MB_OK | MB_ICONERROR);
            return;
        }
        m_is_initialized = true;
    }

    overlay_t::~overlay_t() {}

    void overlay_t::run(callback_t on_paint_callback) {
        m_is_running = true;
        m_on_paint_callback = on_paint_callback;

        while (m_is_running && m_window.process_messages()) {
        }
    }

    LRESULT overlay_t::window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_SIZE:
            if (m_direct2d.get_render_target()) {
                RECT rc;
                GetClientRect(hWnd, &rc);

                D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
                m_direct2d.get_render_target()->Resize(size);

                const auto new_size = math::vec2_t<uint32_t>(size.width, size.height);
                m_window.set_size(new_size);

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
                    MessageBoxA(nullptr, direct2d_initialized.error().c_str(), "[Error]", MB_OK | MB_ICONERROR);
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
                DebugBreak();
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
        return m_window.get_width();
    }

    uint32_t overlay_t::get_height() const {
        return m_window.get_height();
    }

    math::vec2_t<uint32_t> overlay_t::get_size() const {
        return m_window.get_size();
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
            const auto overlay_size = get_size();
            HRESULT hr = m_direct2d.get_dwrite_factory()->CreateTextLayout(
                wstr.c_str(), wstr.size(), font, overlay_size.x, overlay_size.y, &text_layout);

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
            const auto overlay_size = get_size();
            HRESULT hr = m_direct2d.get_dwrite_factory()->CreateTextLayout(
                wstr.c_str(), wstr.size(), font, overlay_size.x, overlay_size.y, &text_layout);

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
