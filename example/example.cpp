#include <fmt/core.h>
#include <overlay_framework/overlay.hpp>

int main() {
    namespace of = overlay_framework;

    // setup the overlay
    const auto overlay_config = of::overlay_config_t{ .window_name = L"ExampleWindow",
                                                      .window_class_name = L"ExampleWindowClassName",
                                                      .vsync = true };

    // create it
    of::overlay_t overlay{ overlay_config };

    // start the main routine
    overlay.run([](of::overlay_t* ov) {
        // draw dynamic text
        {
            POINT pt{};
            GetCursorPos(&pt);

            const auto offset = 25;

            ov->draw_text("hi :3", pt.x - offset, pt.y - offset, 255, 255, 255, 255, true);

            // TODO: bring back support for choosing fonts
            const auto position = of::math::vec2_t<int32_t>{ pt.x, pt.y } + offset;
            ov->draw_text("bye :(", position, of::colors::white, true);
        }

        // draw frame time
        {
            const auto text = fmt::format("frame_time: {:.3f}ms", ov->get_frame_time());
            ov->draw_text(text, 250, 250, 255, 255, 255, 255, true);
        }

        // draw geometry
        {
            auto position = of::math::vec2_t<int32_t>{ 50, 50 };
            const auto size = of::math::vec2_t<int32_t>{ 50, 50 };
            const auto red = 255u;
            const auto green = 0u;
            const auto blue = 0u;
            const auto alpha = 150u;

            // simple
            ov->draw_rect_filled(position.x, position.y, size.x, size.y, red, green, blue, alpha);
            position.y += 100;

            // new
            ov->draw_rect_filled(position, size, { red, green, blue, alpha });
            position.y += 100;

            ov->draw_rect_filled(position, size, of::colors::blue);
        }

        /* place more code here */
    });
}
