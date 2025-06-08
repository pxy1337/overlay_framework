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
        // ov->draw_rect_filled(pt.x, pt.y, 100, 100, 255, 255, 255, 100);
        //
        //
        static POINT position{};
        float smoothing_speed{ 25.f };

        static POINT current_position{};

        float delta_time = 1.f;

        position.x = pt.x;
        position.y = pt.y;
        current_position.x = std::lerp<float>(static_cast<float>(current_position.x), static_cast<float>(position.x), std::clamp<float>(delta_time * smoothing_speed, 0.f, 1.f));
        current_position.y = std::lerp<float>(static_cast<float>(current_position.y), static_cast<float>(position.y), std::clamp<float>(delta_time * smoothing_speed, 0.f, 1.f));

        ov->draw_text("hi :3", current_position.x, current_position.y, 255, 255, 255, 255, true);
    });
}
