add_executable(overlay_example example.cpp)
target_link_libraries(overlay_example PRIVATE overlay fmt::fmt)
target_compile_features(overlay_example PRIVATE cxx_std_23)
target_compile_definitions(overlay_example PRIVATE
    UNICODE _UNICODE WIN32_LEAN_AND_MEAN)
set_target_properties(overlay_example PROPERTIES
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(overlay_example PRIVATE /W4 /WX /MP)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
        VS_STARTUP_PROJECT overlay_example)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(overlay_example PUBLIC
        -Wall -Wextra -Werror -pedantic -Wno-unused-parameter -Wno-unused-variable)
endif()
