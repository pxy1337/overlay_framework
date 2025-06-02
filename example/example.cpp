#include <fmt/core.h>
#include <overlay_framework/overlay.hpp>

int main() {
    namespace of = overlay_framework;

    const auto overlay_config = of::overlay_config_t{ 
        .window_name = "Overlay Window", 
        .window_class_name = "Overlay Window Class name" 
    };

    of::overlay_t overlay{};
    // overlay.run([](of::overlay_t* ov) {
    //     ov->draw_text("hi", {100, 100}, {255, 255, 255, 255});
    // });
}
