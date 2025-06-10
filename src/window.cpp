#include "overlay_framework/window.hpp"
#include "overlay_framework/overlay.hpp"

#include <fmt/format.h>

namespace overlay_framework {
    window_t::~window_t() {
        if (m_window_handle) {
            DestroyWindow(m_window_handle);
            m_window_handle = nullptr;
            UnregisterClass(m_class_name.c_str(), m_instance_handle);
        }
    }

    std::expected<void, std::string> window_t::initialize(overlay_t* ov, const std::wstring& window_name,
                                                          const std::wstring& class_name) {
        m_instance_handle = GetModuleHandle(nullptr);

        WNDCLASSEX wc{};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = static_window_proc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_instance_handle;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(RGB(0, 0, 0));
        wc.lpszMenuName = NULL;
        wc.lpszClassName = class_name.c_str();
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        if (!RegisterClassEx(&wc)) {
            return std::unexpected(fmt::format("RegisterClassEx failed: 0x{:X}", GetLastError()));
        }

        const auto ex_style = WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW;

        m_size = math::vec2_t<uint32_t>(static_cast<int32_t>(GetSystemMetrics(SM_CXSCREEN)),
                                        static_cast<int32_t>(GetSystemMetrics(SM_CYSCREEN)));

        m_window_handle = CreateWindowEx(ex_style, class_name.c_str(), window_name.c_str(), WS_POPUP, 0, 0, m_size.x,
                                         m_size.y, NULL, NULL, wc.hInstance, ov);
        if (!m_window_handle) {
            return std::unexpected(fmt::format("CreateWindowEx failed: 0x{:X}", GetLastError()));
        }

        MARGINS margins = { -1, -1, -1, -1 }; // ?, not sure
        DwmExtendFrameIntoClientArea(m_window_handle, &margins);

        ShowWindow(m_window_handle, SW_SHOW);
        UpdateWindow(m_window_handle);

        SetLayeredWindowAttributes(m_window_handle, 0x000000, 255, LWA_ALPHA);
        return {};
    }

    bool window_t::process_messages() {
        MSG msg{};
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                return false;
        }
        return true;
    }

    LRESULT CALLBACK window_t::static_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_CREATE) {
            LPCREATESTRUCT create_param = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_param->lpCreateParams));
        }

        if (const auto o = reinterpret_cast<overlay_t*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))) {
            return o->window_proc(hwnd, uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

} // namespace overlay_framework
