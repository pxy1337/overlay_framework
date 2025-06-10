add_library(overlay STATIC
    overlay.cpp
    utility.cpp
    direct2d.cpp)

target_link_libraries(overlay PRIVATE fmt::fmt d2d1 dwmapi dwrite)
target_include_directories(overlay PUBLIC ../include)
target_compile_features(overlay PUBLIC cxx_std_23)
set_target_properties(overlay PROPERTIES
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
target_compile_definitions(overlay PUBLIC
    UNICODE _UNICODE WIN32_LEAN_AND_MEAN)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(overlay PRIVATE /W4 /WX /MP)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(overlay PUBLIC
        -Wall -Wextra -Werror -pedantic -Wno-unused-parameter)
endif()
