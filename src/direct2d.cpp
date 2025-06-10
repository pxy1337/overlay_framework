#include "overlay_framework/direct2d.hpp"

#include <fmt/core.h>

#include <overlay_framework/utility.hpp>

namespace overlay_framework {
    std::expected<void, std::string> direct2d_resources_t::initialize(HWND hwnd, bool vsync) noexcept {
        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2d_factory);
        if (FAILED(hr)) {
            return std::unexpected("D2D1CreateFactory failed");
        }

#pragma clang diagnostic ignored "-Wlanguage-extension-token"
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                                 reinterpret_cast<IUnknown**>(&m_dwrite_factory));
#pragma clang diagnostic warning "-Wlanguage-extension-token"
        if (FAILED(hr)) {
            return std::unexpected("DWriteCreateFactory failed");
        }

        RECT rc{};
        if (!GetClientRect(hwnd, &rc)) {
            return std::unexpected(fmt::format("Couldn't retrieve client rect: 0x{:X}", GetLastError()));
        }
        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

        const auto direct2d_vsync = vsync ? D2D1_PRESENT_OPTIONS_NONE : D2D1_PRESENT_OPTIONS_IMMEDIATELY;

        hr = m_d2d_factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                                         D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
            D2D1::HwndRenderTargetProperties(hwnd, size, direct2d_vsync), &m_render_target);
        if (FAILED(hr)) {
            return std::unexpected(fmt::format("CreateHwndRenderTarget failed: 0x{:X}", hr));
        }

        const D2D1_COLOR_F color = D2D1::ColorF(1.f, 1.f, 1.f, 1.f);
        hr = m_render_target->CreateSolidColorBrush(color, &m_brush);
        if (FAILED(hr)) {
            return std::unexpected("CreateSolidColorBrush failed");
        }

        hr =
            m_dwrite_factory->CreateTextFormat(L"Verdana Bold", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL,
                                               DWRITE_FONT_STRETCH_NORMAL, 11.0f, L"en-us", &m_verdana_bold);
        if (FAILED(hr)) {
            return std::unexpected(fmt::format("CreateTextFormat failed: 0x{:X}", GetLastError()));
        }

        hr = m_dwrite_factory->CreateTextFormat(L"Verdana", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
                                                DWRITE_FONT_STRETCH_NORMAL, 11.0f, L"en-us", &m_verdana_regular);
        if (FAILED(hr)) {
            return std::unexpected(fmt::format("CreateTextFormat failed: 0x{:X}", GetLastError()));
        }

        hr = m_verdana_bold->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        if (FAILED(hr)) {
            return std::unexpected("SetTextAlignment failed");
        }

        hr = m_verdana_regular->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        if (FAILED(hr)) {
            return std::unexpected("SetTextAlignment failed");
        }

        // hr =
        // m_verdana_bold->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        // if (FAILED(hr)) {
        //     return std::unexpected("SetParagraphAlignment failed");
        // }

        // m_render_target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
        m_render_target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
        m_render_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

        return {};
    }

    void direct2d_resources_t::cleanup() {
        using namespace utility;
        safe_release(&m_render_target);
        safe_release(&m_d2d_factory);
        safe_release(&m_brush);
        safe_release(&m_dwrite_factory);
        safe_release(&m_verdana_regular);
        safe_release(&m_verdana_bold);
    }
} // namespace overlay_framework
