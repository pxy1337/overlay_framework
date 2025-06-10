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
            ov->draw_text("bye :(", pt.x + offset, pt.y + offset, 255, 255, 255, 255, true);
        }

        // draw frame time
        {
            const auto text = fmt::format("frame_time: {:.3f}ms", ov->get_frame_time());
            ov->draw_text(text, 250, 250, 255, 255, 255, 255, true);
        }

        // draw geometry
        {
            // simple
            ov->draw_rect_filled(50, 50, 50, 50, 255, 0, 0, 150);

            // new
            ov->draw_rect_filled({ 50, 150 }, { 50, 50 }, { 255, 0, 0, 150 });
            ov->draw_rect_filled({ 50, 250 }, { 50, 50 }, of::predefined_colors::blue);
        }

        /* place more code here */
    });
}
