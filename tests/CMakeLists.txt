add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain overlay)
target_include_directories(tests PRIVATE ../include)
target_compile_features(tests PRIVATE cxx_std_23)
set_target_properties(tests PROPERTIES
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
