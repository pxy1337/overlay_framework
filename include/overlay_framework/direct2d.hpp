#pragma once

#include <expected>
#include <string>

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <winuser.h>
#include <dwmapi.h>

namespace overlay_framework {
    class direct2d_resources_t {
    public:
        direct2d_resources_t() = default;
        ~direct2d_resources_t() {
            cleanup();
        }

        direct2d_resources_t(const direct2d_resources_t&) = delete;
        direct2d_resources_t& operator=(const direct2d_resources_t&) = delete;

        direct2d_resources_t(direct2d_resources_t&&) noexcept = default;
        direct2d_resources_t& operator=(direct2d_resources_t&&) noexcept = default;

        [[nodiscard]] std::expected<void, std::string> initialize(HWND hwnd, bool vsync) noexcept;
        void cleanup();

        [[nodiscard]] ID2D1Factory* get_factory() const noexcept {
            return m_d2d_factory;
        }

        [[nodiscard]] ID2D1HwndRenderTarget* get_render_target() const noexcept {
            return m_render_target;
        }

        [[nodiscard]] ID2D1SolidColorBrush* get_brush() const noexcept {
            return m_brush;
        }

        [[nodiscard]] IDWriteFactory* get_dwrite_factory() const noexcept {
            return m_dwrite_factory;
        }

        [[nodiscard]] IDWriteTextFormat* get_verdana_regular() const noexcept {
            return m_verdana_regular;
        }

        [[nodiscard]] IDWriteTextFormat* get_verdana_bold() const noexcept {
            return m_verdana_bold;
        }

    private:
        ID2D1Factory* m_d2d_factory{};
        IDWriteFactory* m_dwrite_factory{};
        ID2D1HwndRenderTarget* m_render_target{};

        IDWriteTextFormat* m_verdana_regular{};
        IDWriteTextFormat* m_verdana_bold{};

        ID2D1SolidColorBrush* m_brush{};
    };
} // namespace overlay_framework
