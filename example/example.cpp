#include <fmt/core.h>
#include <overlay_framework/overlay.hpp>

#include <algorithm>

int main() {
    namespace of = overlay_framework;

    const auto overlay_config =
        of::overlay_config_t{ .window_name = L"ExampleWindow", .window_class_name = L"ExampleWindowClassName" };

    of::overlay_t overlay{ overlay_config };
    overlay.run([](of::overlay_t* ov) {
        POINT pt{};
        GetCursorPos(&pt);

        const auto offset = 50;

        pt.x += offset;
        pt.y += offset;

        ov->draw_text("hi :3", pt.x, pt.y, 255, 255, 255, 255, true);
    });
}
